#pragma once

#include "Engine/Framework/Component.h"
#include "Engine/Framework/Camera.h"
#include "Engine/Framework/Light.h"
#include "Engine/Framework/Billboard.h"

#include <Engine/Core/Log/Logger.h>

#include <memory>
#include <vector>

namespace Engine::Rendering
{
	class MeshRenderer;
}

namespace Engine::Framework::Lights
{
	class PointLight;
	class DirectionalLight;
}

namespace Engine::Framework::Physics
{
	class Collider;
	class PhysicsComponent;
}

namespace Engine::Framework
{
	class Entity;

	class Scene
	{
	public:
		enum SceneState { Edit = 0, Play = 1, Pause = 2 };
	public:
		Scene() : m_DirectionalLight(nullptr), m_SceneCamera(nullptr)
		{
			if (!s_ActiveScene) s_ActiveScene = this;
		}

		virtual ~Scene()
		{
			if (s_ActiveScene == this) s_ActiveScene = nullptr;
		}

		void Init();

		static std::shared_ptr<Scene> Create()
		{
			auto scene = std::make_shared<Scene>();
			CRTN_CHECK_PTR(scene);
			return scene;
		}

		static Scene& Get()
		{
			CRTN_ASSERT(s_ActiveScene, "No active scene!");
			return *s_ActiveScene;
		}

		static void SetActiveScene(Scene* scene) { s_ActiveScene = scene; }

		void SetSceneState(SceneState state) { m_SceneState = state; }
		SceneState& GetSceneState() { return m_SceneState; }

		void AddEntity(std::unique_ptr<Entity> obj);
		void RemoveEntity(Entity* obj);

		void OnUpdateRuntime(float dt) const;
		void OnRender();

		void OnViewportResize(uint32_t width, uint32_t height);

		// ADDERS USED BY COMPONENTS
		void AddCollider(Physics::Collider* collider);
		void AddPhysicsComponent(Physics::PhysicsComponent* physicsComp);
		void AddRenderer(Engine::Rendering::MeshRenderer* renderer);
		void AddPointLight(Lights::PointLight* light);
		void AddBillboard(Billboard* billboard);

		void SetDirectionalLight(Lights::DirectionalLight* directionalLight);

		// REMOVERS USED BY COMPONENTS
		void RemoveCollider(Physics::Collider* collider);
		void RemovePhysicsComponent(Physics::PhysicsComponent* collider);
		void RemoveRenderer(Engine::Rendering::MeshRenderer* renderer);
		void RemovePointLight(Lights::PointLight* pointLight);
		void RemoveBillboard(Billboard* billboard);

		void DeleteDirectionalLight(Lights::DirectionalLight* directionalLight);

		// GETTERS FOR OTHER CLASSES
		const std::vector<std::unique_ptr<Entity>>& GetEntities() const { return m_Entities; }
		const std::vector<Physics::Collider*> GetColliders() const { return m_Colliders; }
		const std::vector<Physics::PhysicsComponent*> GetPhysicsComponents() const { return m_PhysicsComponents; }
		const std::vector<Engine::Rendering::MeshRenderer*> GetRenderers() const { return m_Renderers; }
		const std::vector<Lights::PointLight*> GetPointLights() const { return m_PointLights; }
		const std::vector<Billboard*> GetBillboards() const { return m_Billboards; }

		Lights::DirectionalLight* GetDirectionalLight() { return m_DirectionalLight; }
		const Lights::DirectionalLight* GetDirectionalLight() const { return m_DirectionalLight; }

		Camera* GetSceneCamera() { return m_SceneCamera; }
		const Camera* GetSceneCamera() const { return m_SceneCamera; }
	private:
		std::vector<std::unique_ptr<Entity>> m_Entities;

		// CACHES
		std::vector<Physics::Collider*> m_Colliders;
		std::vector<Physics::PhysicsComponent*> m_PhysicsComponents;
		std::vector<Engine::Rendering::MeshRenderer*> m_Renderers;
		std::vector<Lights::PointLight*> m_PointLights;
		std::vector<Billboard*> m_Billboards;

		Lights::DirectionalLight* m_DirectionalLight;
		Camera* m_SceneCamera;

		static Scene* s_ActiveScene;
		friend class Component;
	private:
		SceneState m_SceneState = SceneState::Edit;
	};
}