#pragma once

#include "Engine/Core/CoreAPI.h"
#include "Engine/Core/Window.h"

#include "Engine/Core/Log/Logger.h"

#include "Engine/Core/Time/Time.h"

#include "Engine/Core/Layer/Layer.h"
#include "Engine/Core/Layer/LayerStack.h"

#include "Engine/Core/Event/WindowEvent.h"
#include "Engine/Core/Event/FramebufferEvent.h"

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

		virtual void Run();

		static Application& Get();

		Window& GetWindow() { return *m_Window; }
		const Window& GetWindow() const { return *m_Window; }
	protected:
		virtual void OnUpdate();
		virtual void OnEvent(Event::Event& e);

		bool OnWindowClose(Event::WindowCloseEvent& e);
		bool OnFramebufferResize(Event::FramebufferResizeEvent e);
	protected:
		Time::Time timestep;
	private:
		bool m_Running = true;

		std::unique_ptr<Window> m_Window;
		Log::Logger m_Logger;
		Layer::LayerStack m_LayerStack;
	};
}