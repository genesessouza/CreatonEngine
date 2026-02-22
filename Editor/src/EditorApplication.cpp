#include "Engine/Editor/EditorApplication.h"
#include "Engine/Editor/EditorLayer.h"

#include <Engine/Core/Event/FramebufferEvent.h>

#include <Engine/Platform/Windows/WindowsWindowFactory.h>
#include <Engine/Platform/Glfw/GlfwTimer.h>

#include <Engine/Sandbox/SandboxLayer.h>

Engine::Core::Application* CreateApplication()
{
	Engine::Core::Window::SetFactory(Engine::Platform::Windows::WindowsWindowFactory);
	Engine::Core::Time::Time::SetProvider(Engine::Platform::Glfw::GlfwGetTimeSeconds);

	auto* app = new Engine::Editor::EditorApplication();
	
	auto* sandboxLayer = new Engine::Sandbox::SandboxLayer();
	app->PushLayer(sandboxLayer);

	auto* editorLayer = new Engine::Editor::EditorLayer();
	app->PushLayer(editorLayer);

	auto [w, h] = Engine::Core::Application::Get().GetWindow().GetWindowSize();
	Engine::Core::Event::FramebufferResizeEvent e(w, h);
	editorLayer->OnEvent(e);

	CRTN_LOG_DEBUG("[EDITOR APP]: EditorApplication ------------- Running\n");

	return app;
}

Engine::Editor::EditorApplication::EditorApplication()
{
	//
}