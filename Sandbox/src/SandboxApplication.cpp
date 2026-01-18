#include "SandboxApplication.h"

#include <Engine/Platform/Windows/WindowsWindowFactory.h>

#include <Engine/Platform/Glfw/GlfwTimer.h>

#include "Layers/SandboxLayer.h"

Engine::Core::Application* CreateApplication()
{
	Engine::Core::Window::SetFactory(Engine::Platform::Windows::WindowsWindowFactory);
	Engine::Core::Time::Time::SetProvider(Engine::Platform::Glfw::GlfwGetTimeSeconds);

	auto* app = new SandboxApplication();
	app->PushLayer(new SandboxLayer());
	return app;
}

SandboxApplication::SandboxApplication()
{
	CRTN_LOG_DEBUG("SandboxApplication Constructor() called");
}