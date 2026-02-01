#pragma once

#include <Engine/Framework/Scene.h>

#include <Engine/Framework/Entity.h>

#include <ImGui/imgui.h> // Full path ---> Engine/Platform/vendor/include/ImGui/ImGui.h

#include <functional>

namespace Engine::Framework
{
	class Scene;
	class Camera;
	class GameObject;

	namespace Physics
	{
		class Collider;
	}
	namespace Lights
	{
		class DirectionalLight;
		class PointLight;
	}
}

namespace Engine::Editor
{
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

		void SelectEntity(Engine::Framework::Entity* entity) { m_SelectedEntity = entity; }
		Engine::Framework::Entity* GetSelectedEntity() const { return m_SelectedEntity; }

		void DrawGameObjectUI(Engine::Framework::GameObject* obj);

		static EditorGUI& Get();
	private:
		ResizeCallback m_ResizeCallback;

		ImVec2 m_ViewportSize = { 0.0f, 0.0f };
		ImVec2 m_ViewportBounds[2];
	private:
		bool m_ImGuiActive = false;
		bool m_ViewportResizing = false;
	private:
		Engine::Framework::Entity* m_SelectedEntity = nullptr;
	};
}