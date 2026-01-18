#include "Engine/Platform/Windows/WindowsWindow.h"

#include <Engine/Core/Application.h>

#include "Engine/Core/Log/Logger.h"

#include <Engine/Core/Event/WindowEvent.h>
#include <Engine/Core/Event/KeyEvent.h>
#include <Engine/Core/Event/MouseEvent.h>
#include <Engine/Core/Event/FramebufferEvent.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine::Platform::Windows
{
	WindowsWindow::WindowsWindow(const Window::WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void WindowsWindow::SetEventCallback(const EventCallbackFn& callback)
	{
		m_EventCallback = callback;
	}

	Engine::Core::Window::FramebufferSize WindowsWindow::GetFramebufferSize()
	{
		int width, height;
		glfwGetFramebufferSize(m_Window, &width, &height);

		return { width, height };
	}

	void WindowsWindow::Init(const Window::WindowProps props)
	{
		int status = glfwInit();
		CRTN_ASSERT(status, "Failed to initialize GLFW!");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), NULL, NULL);
		CRTN_ASSERT(m_Window, "Failed to create GLFW window!");

		m_Context = new Engine::Platform::GraphicsContext(m_Window);
		m_Context->Init();

		/* -------------------------------------------------------------------------------- */
		/* ------------------------------ WINDOW CALLBACKS -------------------------------- */
		/* -------------------------------------------------------------------------------- */

		glfwSetWindowUserPointer(m_Window, this);

		glfwSetWindowPosCallback(m_Window,
			[](GLFWwindow* window, int xpos, int ypos)
			{
				auto* win = (WindowsWindow*)glfwGetWindowUserPointer(window);
				Engine::Core::Event::WindowMovedEvent e(static_cast<uint32_t>(xpos), static_cast<uint32_t>(ypos));
				win->m_EventCallback(e);
			});

		glfwSetWindowMaximizeCallback(m_Window,
			[](GLFWwindow* window, int maximized)
			{
				if (maximized)
				{
					auto* win = (WindowsWindow*)glfwGetWindowUserPointer(window);
					Engine::Core::Event::WindowMaximizeEvent e(maximized);
					win->m_EventCallback(e);
				}
			});

		glfwSetWindowCloseCallback(m_Window,
			[](GLFWwindow* window)
			{
				auto* win = (WindowsWindow*)glfwGetWindowUserPointer(window);
				Engine::Core::Event::WindowCloseEvent e;
				win->m_EventCallback(e);
			});

		glfwSetWindowSizeCallback(m_Window,
			[](GLFWwindow* window, int width, int height)
			{
				auto* win = (WindowsWindow*)glfwGetWindowUserPointer(window);
				Engine::Core::Event::WindowResizeEvent e(width, height);
				win->m_EventCallback(e);
			});

		glfwSetFramebufferSizeCallback(m_Window,
			[](GLFWwindow* window, int width, int height)
			{
				auto* win = (WindowsWindow*)glfwGetWindowUserPointer(window);
				Engine::Core::Event::FramebufferResizeEvent e(width, height);
				win->m_EventCallback(e);
			});

		/* ------------------------------------------------------------------------------------- */
		/* ---------------------------------- MOUSE CALLBACKS ---------------------------------- */
		/* ------------------------------------------------------------------------------------- */

		glfwSetMouseButtonCallback(m_Window,
			[](GLFWwindow* window, int button, int action, int mods)
			{
				auto* win = (WindowsWindow*)glfwGetWindowUserPointer(window);
				if (action == GLFW_PRESS)
				{
					Engine::Core::Event::MouseClickedEvent e(button);
					win->m_EventCallback(e);
				}
			});

		glfwSetCursorPosCallback(m_Window,
			[](GLFWwindow* window, double xpos, double ypos)
			{
				auto* win = (WindowsWindow*)glfwGetWindowUserPointer(window);
				Engine::Core::Event::MouseMovedEvent e(static_cast<float>(xpos), static_cast<float>(ypos));
				win->m_EventCallback(e);
			});

		glfwSetScrollCallback(m_Window,
			[](GLFWwindow* window, double xoffset, double yoffset)
			{
				auto* win = (WindowsWindow*)glfwGetWindowUserPointer(window);
				Engine::Core::Event::MouseScrolledEvent e(static_cast<float>(xoffset), static_cast<float>(yoffset));
				win->m_EventCallback(e);
			});

		/* ------------------------------------------------------------------------------------- */
		/* ---------------------------------- KEY CALLBACKS ------------------------------------ */
		/* ------------------------------------------------------------------------------------- */

		glfwSetKeyCallback(m_Window,
			[](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				auto* win = (WindowsWindow*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						Engine::Core::Event::KeyPressedEvent e(key, 0);
						win->m_EventCallback(e);
						break;
					}
					case GLFW_REPEAT:
					{
						Engine::Core::Event::KeyPressedEvent e(key, 1);
						win->m_EventCallback(e);
						break;
					}
					case GLFW_RELEASE:
					{
						Engine::Core::Event::KeyReleasedEvent e(key);
						win->m_EventCallback(e);
						break;
					}
				}
			});
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	Engine::Core::Window::WindowSize WindowsWindow::GetWindowSize() const
	{
		int width, height;
		glfwGetWindowSize(m_Window, &width, &height);
		return { width, height };
	}

	void WindowsWindow::SetTitle(const std::string& title)
	{
		m_Props.Title = title;
		glfwSetWindowTitle(m_Window, m_Props.Title.c_str());
	}

	void WindowsWindow::Resize(uint32_t width, uint32_t height)
	{
		m_Props.Width = width;
		m_Props.Height = height;

		glfwSetWindowSize(m_Window, width, height);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		
		m_VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_VSync;
	}

	bool WindowsWindow::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}
}