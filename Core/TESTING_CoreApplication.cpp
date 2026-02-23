#include <Engine/Core/Event/FramebufferEvent.h>

#include "TESTING_CoreApplication.h"
#include "Engine/Core/Application.h"
#include "Engine/Core/TESTING_CoreLayer.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

Engine::Core::Application* CreateApplication()
{
	Engine::Core::Window* m_Window; 

	int status = glfwInit();
	if (!status)
	{
		CRTN_LOG_CRITICAL("<CoreApplication::CreateApplication>: GLFW could not be initialized!");
		CRTN_ASSERT(!status, "<CoreApplication::CreateApplication>: Failed to initialize GLFW!");

		return nullptr;
	}

	CRTN_LOG_TRACE("<CoreApplication::CreateApplication>: GLFW initialized");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	m_Window = (Engine::Core::Window*)glfwCreateWindow(800, 600, "CORE TEST", NULL, NULL);

	if (!m_Window)
	{
		CRTN_LOG_CRITICAL("<CoreApplication::CreateApplication>: Failed to create GLFW Window!");
		CRTN_ASSERT(!m_Window, "<CoreApplication::CreateApplication>: Window was not created!");

		return nullptr;
	}

	glfwMakeContextCurrent((GLFWwindow*)m_Window);
	int statusGlad = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	if (!statusGlad)
	{
		CRTN_LOG_CRITICAL("<CoreApplication::CreateApplication>: Failed to run Glad loader!");
		CRTN_ASSERT(!statusGlad, "<CoreApplication::CreateApplication>: Glad not initialized!");

		return nullptr;
	}
	using WindowFactory = std::unique_ptr<Engine::Core::Window>(*)(const Engine::Core::Window::WindowProps&);

	Engine::Core::Window::SetFactory(m_Window);
	Engine::Core::Window::Create({});

	CRTN_LOG_TRACE("<CoreApplication::CreateApplication>: Glad initialized");
	CRTN_LOG_TRACE("<CoreApplication::CreateApplication>: Detected GPU: %s", glGetString(GL_RENDERER));
	CRTN_LOG_TRACE("<CoreApplication::CreateApplication>: OpenGL version: %s", glGetString(GL_VERSION));

	//Engine::Core::Time::Time::SetProvider(Engine::Platform::Glfw::GlfwGetTimeSeconds);

	auto* app = new Engine::Core::Application();

	auto coreLayer = new Engine::Core::CoreLayer();
	app->PushLayer(coreLayer);

	CRTN_LOG_DEBUG("<CoreApplication::CreateApplication>: Testing Core independency ------------- Running\n");

	return app;
}

Engine::Core::CoreApplication::CoreApplication()
{
	//
}