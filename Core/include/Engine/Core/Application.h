#pragma once

#include "Engine/Core/CoreAPI.h"
#include "Engine/Core/Window.h"
#include "Engine/Core/Framebuffer.h"

#include "Engine/Core/Log/Logger.h"

#include "Engine/Core/Time/Time.h"

#include "Engine/Core/Layer/Layer.h"
#include "Engine/Core/Layer/LayerStack.h"

#include "Engine/Core/Event/MouseEvent.h"
#include "Engine/Core/Event/WindowEvent.h"
#include "Engine/Core/Event/FramebufferEvent.h"
#include "Engine/Core/Event/InputState.h"

#include <memory>

namespace Engine::Core
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void PushLayer(Layer::Layer* layer);
		void PushOverlay(Layer::Layer* layer);

		static Application& Get();

		virtual void Run();
		virtual void Close();

		std::unique_ptr<Framebuffer>& GetFramebuffer() { return m_Framebuffer; }

		struct MousePosition
		{
			float XPos;
			float YPos;
		};

		MousePosition GetMousePosition() const
		{
			return MousePosition{ m_MouseState.Position.x, m_MouseState.Position.y };
		}

		Window& GetWindow() { return *m_Window; }
		void* GetNativeWindow() const { return m_Window->GetNativeWindow(); }
	protected:
		virtual void OnUpdate(float deltaTime);
		virtual void OnEvent(Event::Event& e);

		void ProcessInput(float deltaTime);
		void ProcessWindowChanges();

		bool OnMouseMove(Event::MouseMovedEvent& e);
		bool OnMouseScroll(Event::MouseScrolledEvent& e);

		bool OnFramebufferResize(Event::FramebufferResizeEvent e);

		bool OnWindowMove(Event::WindowMoveEvent& e);
		bool OnWindowResize(Event::WindowResizeEvent& e);
		bool OnWindowClose(Event::WindowCloseEvent& e);
	protected:
		Time::Time timestep;
	private:
		bool m_Running = true;
	private:
		std::unique_ptr<Window> m_Window;
		Log::Logger m_Logger;
		Layer::LayerStack m_LayerStack;
	private:
		Event::State::MouseState m_MouseState;
		Event::State::WindowState m_WindowState;
		Event::State::FramebufferState m_FramebufferState;
	private:
		std::unique_ptr<Framebuffer> m_Framebuffer;
	};
}