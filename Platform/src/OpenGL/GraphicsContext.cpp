#include "Engine/Platform/OpenGL/GraphicsContext.h"

#include <Engine/Core/Log/Logger.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine::Platform
{
	GraphicsContext::GraphicsContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		CRTN_ASSERT(windowHandle, "Window handle is null!");
	}

	void GraphicsContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CRTN_ASSERT(status, "Failed to initialize Glad!");
	}

	void GraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}