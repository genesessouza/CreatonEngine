#include "Engine/Platform/OpenGL/GraphicsContext.h"

#include <Engine/Core/Log/Logger.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

namespace Engine::Platform
{
	GraphicsContext::GraphicsContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		if (!windowHandle)
		{
			CRTN_LOG_CRITICAL("<GraphicsContext::GraphicsContext>: [m_WindowHandle] is null!");
			CRTN_ASSERT(!windowHandle, "<GraphicsContext::GraphicsContext>: Window handle not set!");

			return;
		}
	}

	void GraphicsContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		if (!status)
		{
			CRTN_LOG_CRITICAL("<GraphicsContext::Init>: Failed to run Glad loader!");
			CRTN_ASSERT(!status, "<GraphicsContext::Init>: Glad not initialized!");

			return;
		}

		CRTN_LOG_TRACE("<GraphicsContext::Init>: Glad initialized");
		CRTN_LOG_TRACE("<GraphicsContext::Init>: Detected GPU: %s", glGetString(GL_RENDERER));
		CRTN_LOG_TRACE("<GraphicsContext::Init>: OpenGL version: %s", glGetString(GL_VERSION));

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		CRTN_LOG_TRACE("<GraphicsContext::Init>: Depth Test enabled");

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		CRTN_LOG_TRACE("<GraphicsContext::Init>: Face Culling::Back enabled\n");
	}

	void GraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}