/*
#include "Engine/Sandbox/SandboxApplication.h"
#include "Engine/Sandbox/SandboxLayer.h"

#include <Engine/Platform/Windows/WindowsWindowFactory.h>

#include <Engine/Platform/Glfw/GlfwTimer.h>

Engine::Core::Application* CreateApplication()
{
	Engine::Core::Window::SetFactory(Engine::Platform::Windows::WindowsWindowFactory);
	Engine::Core::Time::Time::SetProvider(Engine::Platform::Glfw::GlfwGetTimeSeconds);

	auto* app = new Engine::Sandbox::SandboxApplication();
	auto* sandboxLayer = new Engine::Sandbox::SandboxLayer();
	app->PushLayer(sandboxLayer);

	auto [w, h] = Engine::Core::Application::Get().GetWindow().GetFramebufferSize();
	Engine::Core::Event::FramebufferResizeEvent e(w, h);
	sandboxLayer->OnEvent(e);

	CRTN_LOG_DEBUG("[SANDBOX APP]: SandboxApplication ------------- Running\n");

	return app;
}

Engine::Sandbox::SandboxApplication::SandboxApplication()
{
	//
}
*/