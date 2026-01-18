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

		m_Window = Window::Create(Window::WindowProps{});
		m_Window->SetEventCallback([this](Event::Event& e) { OnEvent(e); });
		m_Window->SetVSync(false);

		auto [w, h] = m_Window->GetFramebufferSize();
        Event::FramebufferResizeEvent framebufferResizeEvent(w, h);
        OnEvent(framebufferResizeEvent);

		m_Running = true;
	}

	Application::~Application()
	{
		s_Instance = nullptr;
	}

	void Application::PushLayer(Layer::Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
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

			if (fpsTimer >= 0.5f)
			{
				float fps = frameCount / fpsTimer;

				char title[128];
				snprintf(title, sizeof(title), "CreationEngine | FPS: %.2f");
				m_Window->SetTitle(title);

				fpsTimer = 0.0f;
				frameCount = 0;
			}

			OnUpdate();
			m_Window->OnUpdate();

			for (Layer::Layer* layer : m_LayerStack)
				layer->OnUpdate(deltaTime);
		}
	}

	Application& Application::Get()
	{
		return *s_Instance;
	}

	bool Application::OnFramebufferResize(Event::FramebufferResizeEvent e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
			return false;

		RenderCommand::SetViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}

	void Application::OnUpdate()
	{
		// Nothing for now
	}

	void Application::OnEvent(Event::Event& e)
	{
		Event::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Event::WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		dispatcher.Dispatch<Event::FramebufferResizeEvent>(BIND_EVENT_FN(Application::OnFramebufferResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvent(e);
			if (e.Handled)
				break;
		}

		//CRTN_LOG_TRACE("Application registered event: <%s>", e.ToString().c_str());
	}

	bool Application::OnWindowClose(Event::WindowCloseEvent& e)
	{
		m_Running = false;
		return m_Running;
	}
}