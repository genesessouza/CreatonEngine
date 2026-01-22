#pragma once

#include <ImGui/imgui.h> // Full path ---> Engine/Platform/vendor/include/ImGui/ImGui.h

namespace Engine::Editor
{
	class EditorGUI
	{
	public:
		EditorGUI();
		virtual ~EditorGUI() = default;

		void Init();

	private:
		bool m_ImGuiActive = false;
	};
}