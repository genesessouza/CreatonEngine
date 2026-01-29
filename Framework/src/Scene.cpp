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
		auto mainCam = Camera::Create("[3D Camera] Main");
		m_SceneCamera = mainCam.get();
		AddEntity(std::move(mainCam));

		auto dirLightGO = GameObject::Create("[Directional Light] Sun");
		dirLightGO->AddComponent<Lights::DirectionalLight>();

		auto dirLightComp = dirLightGO->GetComponent<Lights::DirectionalLight>();
		dirLightComp->SetOwner(dirLightGO.get());

		dirLightComp->Init();
		m_DirectionalLight = dirLightComp;

		SetDirectionalLight(m_DirectionalLight);
		AddEntity(std::move(dirLightGO));
	}

	void Scene::AddEntity(std::unique_ptr<Engine::Framework::Entity> obj)
	{
		m_Entities.push_back(std::move(obj));
	}

	void Scene::RemoveEntity(Entity* obj)
	{
		m_Entities.erase(
			std::remove_if(m_Entities.begin(), m_Entities.end(),
				[obj](const std::unique_ptr<Entity>& e) { return e.get() == obj; }),
			m_Entities.end()
		);
	}

	void Scene::AddCollider(Physics::Collider* collider)
	{
		m_Colliders.push_back(collider);
	}
	void Scene::AddPhysicsComponent(Physics::PhysicsComponent* physicsComp)
	{
		m_PhysicsComponents.push_back(physicsComp);
	}
	void Scene::AddRenderable(Engine::Rendering::MeshRenderer* renderable)
	{
		m_Renderables.push_back(renderable);
	}
	void Scene::AddPointLight(Lights::PointLight* pointLight)
	{
		m_PointLights.push_back(pointLight);
	}

	void Scene::SetDirectionalLight(Lights::DirectionalLight* directionalLight)
	{
		//m_DirectionalLight = directionalLight;
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
	void Scene::RemoveRenderable(Engine::Rendering::MeshRenderer* renderable)
	{
		m_Renderables.erase(std::remove(m_Renderables.begin(), m_Renderables.end(), renderable), m_Renderables.end());
	}
	void Scene::RemovePointLight(Lights::PointLight* pointLight)
	{
		m_PointLights.erase(std::remove(m_PointLights.begin(), m_PointLights.end(), pointLight), m_PointLights.end());
	}

	void Scene::DeleteDirectionalLight(Lights::DirectionalLight* directionalLight)
	{
		//directionalLight = nullptr;
	}

	void Scene::OnUpdateRuntime(float dt) const
	{
		for (auto& obj : m_Entities)
			obj->OnUpdate();

		Physics::PhysicsSystem::UpdateCaches(*this);
		Physics::PhysicsSystem::Step(dt);
	}

	void Scene::OnRender()
	{
		Engine::Rendering::Renderer::BeginScene(*m_SceneCamera, *this);

		for (auto& renderer : m_Renderables)
		{
			auto& transform = renderer->GetOwner()->GetTransform();
			renderer->Draw(transform);
		}

		Engine::Rendering::Renderer::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_SceneCamera->SetViewportSize(width, height);
	}
}