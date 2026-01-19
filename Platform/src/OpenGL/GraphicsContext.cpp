#include "Engine/Platform/OpenGL/GraphicsContext.h"

#include <Engine/Core/Log/Logger.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

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

		CRTN_LOG_DEBUG("[GRAPHICS CONTEXT]: Glad initialized");
		CRTN_LOG_TRACE("[GRAPHICS CONTEXT]: Detected GPU: %s", glGetString(GL_RENDERER));
		CRTN_LOG_TRACE("[GRAPHICS CONTEXT]: OpenGL version: %s", glGetString(GL_VERSION));

		glEnable(GL_DEPTH_TEST);

		CRTN_LOG_INFO("[GRAPHICS CONTEXT]: Depth Test enabled");

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		CRTN_LOG_INFO("[GRAPHICS CONTEXT]: Face Culling::Back enabled\n");
	}

	void GraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}