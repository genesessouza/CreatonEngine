#include "Engine/Core/Application.h"
#include "Engine/Core/Window.h"
#include "Engine/Core/RenderCommand.h"

#include "Engine/Core/Event/WindowEvent.h"

namespace Engine::Core
{
	static Application* s_Instance = nullptr;

	Application::Application()
	{
		CRTN_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		CRTN_LOG_DEBUG("[APPLICATION]: Base application initialized");
		CRTN_LOG_DEBUG("[APPLICATION]: Opening application window\n");

		m_Window = Window::Create(Window::WindowProps{});
		m_Window->SetEventCallback([this](Event::Event& e) { OnEvent(e); });
		m_Window->SetVSync(false);

		m_Running = true;

		m_Framebuffer = Framebuffer::Create(m_Window->GetFramebufferSize().width, m_Window->GetFramebufferSize().height);
	}

	Application::~Application()
	{
		s_Instance = nullptr;
	}

	Application& Application::Get()
	{
		return *s_Instance;
	}

	void Application::PushLayer(Layer::Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	void Application::PushOverlay(Layer::Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	void Application::Run()
	{
		float lastTime = Time::Time::GetTime();
		float fpsTimer = 0.0f;
		uint32_t frameCount = 0;

		while (m_Running)
		{
			float currentTime = Time::Time::GetTime();
			float deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			fpsTimer += deltaTime;
			frameCount++;

			if (fpsTimer >= 1.0f)
			{
				float fps = frameCount / fpsTimer;

				char title[128];
				snprintf(
					title, sizeof(title), "CreationEngine - [%d x %d] | FPS: %.2f - %.3fms",
					m_Window->GetWindowSize().width, m_Window->GetWindowSize().height, fps,
					(fpsTimer / frameCount) * 1000.0f
				);
				m_Window->SetTitle(title);

				fpsTimer = 0.0f;
				frameCount = 0;
			}

			if (m_FramebufferState.Resized)
			{
				m_Framebuffer->Resize(m_FramebufferState.Width, m_FramebufferState.Height);
				m_FramebufferState.Resized = false;
			}

			static float timeAccumulator = 0.0f;
			timeAccumulator += deltaTime;

			if (timeAccumulator >= 0.016f)
			{
				// SCENE RENDERING
				m_Framebuffer->Bind();

				RenderCommand::SetViewport(0, 0, m_Framebuffer->GetWidth(), m_Framebuffer->GetHeight());

				for (Layer::Layer* layer : m_LayerStack)
				{
					if (m_SceneState == SceneState::State::Play)
						layer->OnUpdate(deltaTime);
					else
						layer->OnEditorUpdate(deltaTime);
				}

				m_Framebuffer->Unbind();
			}

			// UI RENDERING
			auto [w, h] = m_Window->GetWindowSize();

			RenderCommand::SetViewport(0, 0, w, h);
			RenderCommand::ClearUI(); // Prevents scene viewport color from leaking to UI borders

			RenderCommand::BeginGUI();
			for (Layer::Layer* layer : m_LayerStack)
				layer->OnGUIUpdate();
			RenderCommand::EndGUI();

			OnUpdate(deltaTime);
			m_Window->OnUpdate();
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnUpdate(float deltaTime)
	{
		ProcessInput(deltaTime);
		ProcessWindowChanges();

		if (m_FramebufferState.Resized)
		{
			RenderCommand::SetViewport(
				0, 0,
				m_FramebufferState.Width,
				m_FramebufferState.Height
			);

			//RenderCommand::SetViewport(0, 0, m_Window->GetWindowSize().width, m_Window->GetWindowSize().height);
			Engine::Core::RenderCommand::MarkFramebufferDirty();
			m_FramebufferState.Resized = false;
		}
	}

	void Application::OnEvent(Event::Event& e)
	{
		Event::EventDispatcher dispatcher(e);

		dispatcher.Dispatch<Event::WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<Event::WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
		dispatcher.Dispatch<Event::WindowMoveEvent>(BIND_EVENT_FN(Application::OnWindowMove));

		dispatcher.Dispatch<Event::FramebufferResizeEvent>(BIND_EVENT_FN(Application::OnFramebufferResize));

		dispatcher.Dispatch<Event::MouseMovedEvent>(BIND_EVENT_FN(Application::OnMouseMove));
		dispatcher.Dispatch<Event::MouseScrolledEvent>(BIND_EVENT_FN(Application::OnMouseScroll));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvent(e);
			if (e.Handled)
				break;
		}

		//CRTN_LOG_TRACE("Application registered event: <%s>", e.ToString().c_str());
	}

	void Application::ProcessInput(float deltaTime)
	{
		if (m_MouseState.Moved)
			m_MouseState.Moved = false;

		if (m_MouseState.Scrolled)
			m_MouseState.Scrolled = false;
	}

	void Application::ProcessWindowChanges()
	{
		if (m_WindowState.Resized)
		{
			RenderCommand::SetViewport(0, 0, m_Window->GetWindowSize().width, m_Window->GetWindowSize().height);
			m_WindowState.Resized = false;
		}
	}

	bool Application::OnMouseMove(Event::MouseMovedEvent& e)
	{
		m_MouseState.Delta = { e.GetX() - m_MouseState.Position.x, e.GetY() - m_MouseState.Position.y };

		m_MouseState.Position = { e.GetX(), e.GetY() };

		m_MouseState.Moved = true;

		return false;
	}

	bool Application::OnMouseScroll(Event::MouseScrolledEvent& e)
	{
		m_MouseState.ScrollDelta = e.GetYOffset();

		m_MouseState.Scrolled = true;

		return false;
	}

	bool Application::OnFramebufferResize(Event::FramebufferResizeEvent e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
			return false;

		m_FramebufferState.Width = e.GetWidth();
		m_FramebufferState.Height = e.GetHeight();

		m_FramebufferState.Resized = true;
		return true;
	}

	bool Application::OnWindowMove(Event::WindowMoveEvent& e)
	{
		m_WindowState.PosX = e.GetX();
		m_WindowState.PosY = e.GetY();

		m_WindowState.Moved = true;

		return false;
	}

	bool Application::OnWindowResize(Event::WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
			return false;

		m_WindowState.Width = e.GetWidth();
		m_WindowState.Height = e.GetHeight();

		m_WindowState.Resized = true;

		return false;
	}

	bool Application::OnWindowClose(Event::WindowCloseEvent& e)
	{
		m_Running = false;

		return m_Running;
	}
}