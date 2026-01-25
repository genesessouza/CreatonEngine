#pragma once

#include <Engine/Core/SceneState.h>

#include <Engine/Framework/Entity.h>
#include <Engine/Framework/Light.h>

#include <ImGui/imgui.h> // Full path ---> Engine/Platform/vendor/include/ImGui/ImGui.h

#include <functional>

namespace Engine::Editor
{
	class Scene;

	class EditorGUI
	{
	public:
		EditorGUI();
		virtual ~EditorGUI() = default;

		using ResizeCallback = std::function<void(uint32_t, uint32_t)>;
		void SetResizeCallback(const ResizeCallback& callback) { m_ResizeCallback = callback; }

		const ImVec2* GetViewportBounds() const { return m_ViewportBounds; }

		void OnInit();
		void OnGUIRender();
		void OnShutdown();

		void DrawNavbar();
		void DrawHierarchy();
		void DrawSceneViewport();
		void DrawConsole();
		void DrawObjectInfo();

		void SelectEntity(const std::shared_ptr<Engine::Framework::Entity>& entity) { m_SelectedEntity = entity; }

		void DrawPointLightUI(std::shared_ptr<Engine::Framework::Lights::PointLight> light);
		void DrawDirectionalLightUI(std::shared_ptr<Engine::Framework::Lights::DirectionalLight> light);
		void DrawGameObjectUI(std::shared_ptr<Engine::Framework::GameObject> obj);

		static EditorGUI& Get();
	private:
		ResizeCallback m_ResizeCallback;

		ImVec2 m_ViewportSize = { 0.0f, 0.0f };
		ImVec2 m_ViewportBounds[2];
	private:
		bool m_ImGuiActive = false;
		bool m_ViewportResizing = false;

		Engine::Core::SceneState::State m_SceneState = Engine::Core::SceneState::State::Edit;
	private:
		std::shared_ptr<Engine::Framework::Entity> m_SelectedEntity = nullptr;
	};
}