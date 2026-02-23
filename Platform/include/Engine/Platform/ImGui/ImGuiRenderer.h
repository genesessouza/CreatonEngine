#pragma once

namespace Engine::Platform::GUI
{
	class ImGuiRenderer
	{
	public:
		ImGuiRenderer() = default;
		virtual ~ImGuiRenderer() = default;
		
		static void ClearUI();

		static void Begin();
		static void End();

		static ImGuiRenderer* Create() { return new Engine::Platform::GUI::ImGuiRenderer(); }
	};
}