#include "Engine/Rendering/GUIRenderer.h"

#include <Engine/Platform/ImGui/ImGuiRenderer.h>

namespace Engine::Rendering
{
	std::unique_ptr<GUIRenderer> GUIRenderer::Create()
	{
		return std::make_unique<Engine::Platform::GUI::ImGuiRenderer>();
	}
}