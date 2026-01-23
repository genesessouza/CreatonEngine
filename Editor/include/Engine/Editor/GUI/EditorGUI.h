#pragma once

#include <Engine/Core/SceneState.h>

#include <ImGui/imgui.h> // Full path ---> Engine/Platform/vendor/include/ImGui/ImGui.h

#include <functional>

namespace Engine::Editor
{
	class EditorGUI
	{
	public:
		EditorGUI() = default;
		virtual ~EditorGUI() = default;

		using ResizeCallback = std::function<void(uint32_t, uint32_t)>;
		void SetResizeCallback(const ResizeCallback& callback) { m_ResizeCallback = callback; }

		void OnInit();
		void OnGUIRender();
		void OnShutdown();
	private:
		ResizeCallback m_ResizeCallback;
		ImVec2 m_ViewportSize = { 0.0f, 0.0f };
	private:
		bool m_ImGuiActive = false;
		bool m_ViewportResizing = false;

		Engine::Core::SceneState::State m_SceneState = Engine::Core::SceneState::State::Edit;
	};
}