#pragma once

#include <Engine/Rendering/GUIRenderer.h>

namespace Engine::Platform::GUI
{
	class ImGuiRenderer : public Engine::Rendering::GUIRenderer
	{
	public:
		ImGuiRenderer();
		virtual ~ImGuiRenderer() = default;
		
		void ClearUI() override;

		void Begin() override;
		void End() override;

		static ImGuiRenderer& Get();
	};
}