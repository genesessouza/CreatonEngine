#pragma once

#include "Engine/Platform/ImGui/ImGuiRenderer.h"

namespace Engine::Editor
{
	class EditorStyle
	{
	public:
		virtual ~EditorStyle() = default;

		static void ApplyDarkTheme()
		{
			Engine::Platform::GUI::ImGuiRenderer::Get()->ClearUI();
		}
	};
}