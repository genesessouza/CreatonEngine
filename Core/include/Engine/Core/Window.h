#pragma once

#include "Engine/Core/CoreAPI.h"

#include "Engine/Core/Log/Logger.h"

#include "Engine/Core/Event/Event.h"

#include <memory>
#include <string>
#include <cstdint>
#include <functional>

namespace Engine::Core
{
	class CORE_API Window
	{
	public:
		struct WindowProps
		{
			std::string Title = "Creation Engine";
			uint32_t Width = 800;
			uint32_t Height = 600;
		};
		struct WindowSize
		{
			int width;
			int height;
		};
		struct FramebufferSize
		{
			int width;
			int height;
		};
	public:
		Window() = default;
		virtual ~Window() = default;

		using WindowFactory = std::unique_ptr<Window>(*)(const WindowProps&);

		static std::unique_ptr<Window> Create(const WindowProps& props);
		static void SetFactory(WindowFactory factory);

		using EventCallbackFn = std::function<void(Event::Event&)>;
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		virtual FramebufferSize GetFramebufferSize() = 0;

		virtual void Init(const Window::WindowProps props) = 0;

		virtual void OnUpdate() = 0;

		virtual WindowSize GetWindowSize() const = 0;

		virtual void SetTitle(const std::string& title) = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		const WindowProps& GetProps() const { return m_Props; }
	
		virtual bool ShouldClose() const = 0;
	protected:
		WindowProps m_Props;
		bool m_VSync = false;

		std::unique_ptr<EventCallbackFn> m_EventCallback;
	};
}