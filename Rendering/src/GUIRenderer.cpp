#include "Engine/Rendering/GUIRenderer.h"

#include <Engine/Platform/ImGui/ImGuiRenderer.h>

namespace Engine::Rendering
{
	GUIRenderer* GUIRenderer::s_Instance = nullptr;

	GUIRenderer* GUIRenderer::Create()
	{
		return new Engine::Platform::GUI::ImGuiRenderer();
	}
}