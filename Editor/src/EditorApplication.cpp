#include "Engine/Editor/EditorApplication.h"
#include "Engine/Editor/EditorLayer.h"

#include <Engine/Platform/Windows/WindowsWindowFactory.h>

#include <Engine/Platform/Glfw/GlfwTimer.h>

Engine::Core::Application* CreateApplication()
{
	Engine::Core::Window::SetFactory(Engine::Platform::Windows::WindowsWindowFactory);
	Engine::Core::Time::Time::SetProvider(Engine::Platform::Glfw::GlfwGetTimeSeconds);

	auto* app = new Engine::Editor::EditorApplication();
	auto* editorLayer = new Engine::Editor::EditorLayer();
	app->PushLayer(editorLayer);

	auto [w, h] = Engine::Core::Application::Get().GetWindow().GetFramebufferSize();
	Engine::Core::Event::FramebufferResizeEvent e(w, h);
	editorLayer->OnEvent(e);

	CRTN_LOG_DEBUG("[EDITOR APP]: EditorApplication ------------- Running\n");

	return app;
}

Engine::Editor::EditorApplication::EditorApplication()
{
	//
}