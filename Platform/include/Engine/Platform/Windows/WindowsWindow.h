#pragma once

#include <Engine/Core/Window.h>

#include "Engine/Platform/OpenGL/GraphicsContext.h"

struct GLFWwindow;

namespace Engine::Platform::Windows
{
	class WindowsWindow : public Engine::Core::Window
	{
	public:
		WindowsWindow(const Engine::Core::Window::WindowProps& props);
		~WindowsWindow() override;

		void SetEventCallback(const EventCallbackFn& callback) override;

		void Init(const Window::WindowProps props) override;

		void OnUpdate() override;

		WindowSize GetWindowSize() const override;

		void SetTitle(const std::string& title) override;
		void Resize(uint32_t width, uint32_t height) override;

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		bool ShouldClose() const override;

		virtual void* GetNativeWindow() const override
		{
			if (!m_Window)
			{
				CRTN_LOG_CRITICAL("<WindowsWindow::GetNativeWindow>: [m_Window] is null pointer!");
				CRTN_ASSERT(!m_Window, "<WindowsWindow::GetNativeWindow>: Failed to get native window!");
			}

			return m_Window;
		}
	private:
		GLFWwindow* m_Window = nullptr;

		GraphicsContext* m_Context;

		EventCallbackFn m_EventCallback;
	};
}