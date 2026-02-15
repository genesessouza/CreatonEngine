#include "Engine/Framework/Scene.h"

#include "Engine/Framework/Entity.h"
#include "Engine/Framework/Light.h"

#include "Engine/Framework/Physics/PhysicsSystem.h"
#include "Engine/Framework/Physics/PhysicsComponent.h"
#include "Engine/Framework/Physics/Collider.h"

#include <Engine/Rendering/MeshRenderer.h>
#include <Engine/Rendering/Renderer.h>

namespace Engine::Framework
{
	Scene* Scene::s_ActiveScene = nullptr;

	void Scene::Init()
	{
		// MAIN CAMERA
		{
			auto mainCamGO = Entity::CreateBillboard("[Entity] Main Camera", 1.0f, glm::vec4(1.0f));

			auto mainCam = mainCamGO->AddComponent<Camera>();
			mainCam->Init();

			m_SceneCamera = mainCam;
			AddEntity(std::move(mainCamGO));
		}

		// DIRECTIONAL LIGHT
		{
			auto dirLightGO = Entity::CreateBillboard("[Entity] Directional Light", 1.0f, glm::vec4(1.0f));

			auto dirLight = dirLightGO->AddComponent<Lights::DirectionalLight>();
			dirLight->Init();

			m_DirectionalLight = dirLight;

			SetDirectionalLight(m_DirectionalLight);
			AddEntity(std::move(dirLightGO));
		}
	}

	void Scene::AddEntity(std::unique_ptr<Entity> entity)
	{
		if (auto* dirLight = entity->GetComponent<Lights::DirectionalLight>())
			SetDirectionalLight(dirLight);

		for (auto* pointLight : entity->GetComponents<Lights::PointLight>())
			AddPointLight(pointLight);

		for (auto* billboard : entity->GetComponents<Billboard>())
			AddBillboard(billboard);

		for (auto* renderable : entity->GetComponents<Engine::Rendering::MeshRenderer>())
			AddRenderer(renderable);

		for (auto* collider : entity->GetComponents<Physics::Collider>())
			AddCollider(collider);

		for (auto* physics : entity->GetComponents<Physics::PhysicsComponent>())
			AddPhysicsComponent(physics);

		m_Entities.push_back(std::move(entity));
	}

	void Scene::RemoveEntity(Entity* entity)
	{
		for (auto* renderable : entity->GetComponents<Engine::Rendering::MeshRenderer>())
			RemoveRenderer(renderable);

		for (auto* collider : entity->GetComponents<Physics::Collider>())
			RemoveCollider(collider);

		for (auto* physics : entity->GetComponents<Physics::PhysicsComponent>())
			RemovePhysicsComponent(physics);

		m_Entities.erase(std::remove_if(m_Entities.begin(), m_Entities.end(), [entity](const std::unique_ptr<Entity>& e) { return e.get() == entity; }), m_Entities.end());
	}
	void Scene::AddRenderer(Engine::Rendering::MeshRenderer* renderer)
	{
		m_Renderers.push_back(renderer);
	}

	void Scene::AddCollider(Physics::Collider* collider)
	{
		m_Colliders.push_back(collider);
	}
	void Scene::AddPhysicsComponent(Physics::PhysicsComponent* physicsComp)
	{
		m_PhysicsComponents.push_back(physicsComp);
	}
	void Scene::AddPointLight(Lights::PointLight* pointLight)
	{
		m_PointLights.push_back(pointLight);
	}
	void Scene::AddBillboard(Billboard* billboard)
	{
		m_Billboards.push_back(billboard);

	}

	void Scene::SetDirectionalLight(Lights::DirectionalLight* directionalLight)
	{
		m_DirectionalLight = directionalLight;
	}

	// REMOVERS USED BY COMPONENTS
	void Scene::RemoveCollider(Physics::Collider* collider)
	{
		m_Colliders.erase(std::remove(m_Colliders.begin(), m_Colliders.end(), collider), m_Colliders.end());
	}
	void Scene::RemovePhysicsComponent(Physics::PhysicsComponent* physicsComp)
	{
		m_PhysicsComponents.erase(std::remove(m_PhysicsComponents.begin(), m_PhysicsComponents.end(), physicsComp), m_PhysicsComponents.end());
	}
	void Scene::RemoveRenderer(Engine::Rendering::MeshRenderer* renderer)
	{
		m_Renderers.erase(std::remove(m_Renderers.begin(), m_Renderers.end(), renderer), m_Renderers.end());
	}
	void Scene::RemovePointLight(Lights::PointLight* pointLight)
	{
		m_PointLights.erase(std::remove(m_PointLights.begin(), m_PointLights.end(), pointLight), m_PointLights.end());
	}
	void Scene::RemoveBillboard(Billboard* billboard)
	{
		m_Billboards.erase(std::remove(m_Billboards.begin(), m_Billboards.end(), billboard), m_Billboards.end());
	}

	void Scene::DeleteDirectionalLight(Lights::DirectionalLight* directionalLight)
	{
		//directionalLight = nullptr;
	}

	void Scene::OnUpdateRuntime(float dt) const
	{
		Physics::PhysicsSystem::UpdateCaches(*this);
		if (m_SceneState == SceneState::Play)
		{
			for (auto& obj : m_Entities)
				obj->OnUpdate();

			Physics::PhysicsSystem::Step(dt);
		}
	}

	void Scene::OnRender()
	{
		Engine::Rendering::Renderer::BeginScene(*m_SceneCamera, *this);

		for (auto& renderer : m_Renderers)
		{
			auto& transform = renderer->GetOwner()->GetTransform();

			if (!renderer->IsEnabled() || !renderer->GetOwner()->IsEnabled()) continue;
			renderer->Draw(transform);
		}

		for (auto& billboard : m_Billboards)
		{
			auto& transform = billboard->GetOwner()->GetTransform();
			billboard->Draw(transform, *m_SceneCamera);
		}

		Engine::Rendering::Renderer::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_SceneCamera->SetViewportSize(width, height);
	}
}