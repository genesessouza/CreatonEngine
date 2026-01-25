#include "Engine/Framework/Scene.h"

#include "Engine/Framework/Physics/Collider.h"
#include <Engine/Framework/Debugging.h>

#include <algorithm>

namespace Engine::Framework
{
	Engine::Framework::Scene* Engine::Framework::Scene::s_ActiveScene = nullptr;

	const std::vector<std::shared_ptr<Lights::PointLight>> Scene::GetPointLights() const
	{
		std::vector<std::shared_ptr<Lights::PointLight>> lights;
		for (auto& e : m_Entities) {
			if (auto l = std::dynamic_pointer_cast<Lights::PointLight>(e))
				lights.push_back(l);
		}
		return lights;
	}

	const std::shared_ptr<Lights::DirectionalLight> Scene::GetDirectionalLight() const
	{
		for (auto& e : m_Entities) {
			if (auto dl = std::dynamic_pointer_cast<Lights::DirectionalLight>(e))
				return dl;
		}
		return nullptr;
	}

	void Scene::OnUpdateRuntime(float ts)
	{
		for (auto& e : m_Entities)
		{
			auto obj = std::dynamic_pointer_cast<GameObject>(e);
			if (!obj || !obj->IsEnabled()) continue;

			if (auto phys = obj->GetComponent<Physics::PhysicsComponent>())
			{
				if (phys->IsStatic() || !phys->IsEnabled()) continue;

				phys->ApplyGravity(ts);

				glm::vec3 currentPos = obj->GetTransform().GetPosition();
				glm::vec3 velocity = phys->GetVelocity();

				obj->GetTransform().SetPosition(currentPos + velocity * ts);
				obj->GetTransform().SetRotation(phys->GetAngularVelocity() * ts);
			}
		}

		for (size_t i = 0; i < m_Entities.size(); i++)
		{
			auto objA = std::dynamic_pointer_cast<GameObject>(m_Entities[i]);
			if (!objA || !objA->IsEnabled()) continue;

			for (size_t j = i + 1; j < m_Entities.size(); j++)
			{
				auto objB = std::dynamic_pointer_cast<GameObject>(m_Entities[j]);
				if (!objB || !objB->IsEnabled()) continue;

				auto colA = objA->GetComponent<Physics::Collider>();
				auto colB = objB->GetComponent<Physics::Collider>();

				if (colA && colB)
				{
					if(!colA->IsEnabled() || !colB->IsEnabled())
						continue;

					if (colA->IsStatic() && colB->IsStatic())
						continue;

					if (colA->IsTrigger() || colB->IsTrigger())
						continue;

					glm::vec3 overlap(0.0f);
					glm::vec3 hitPoint(0.0f);

					if (colA->CheckCollision(colB.get(), overlap, hitPoint))
					{
						auto physA = objA->GetComponent<Physics::PhysicsComponent>();
						auto physB = objB->GetComponent<Physics::PhysicsComponent>();

						ResolveCollision(physA.get(), physB.get(), overlap, hitPoint);

						Engine::Framework::Debugging::AddHitPoint(hitPoint, 0.1f, { 1.0f, 0.0f, 0.0f, 1.0f }, 1.0f);
					}
				}
			}
		}
	}

	void Scene::ResolveCollision(Physics::PhysicsComponent* a, Physics::PhysicsComponent* b, const glm::vec3& overlap, const glm::vec3& hitPoint)
	{
		auto physA = a;
		auto physB = b;

		float invMassA = a->GetInverseMass();
		float invMassB = b ? b->GetInverseMass() : 0.0f;
		float totalInvMass = invMassA + invMassB;

		if (totalInvMass == 0) return;

		glm::vec3 normal = glm::normalize(overlap);

		glm::vec3 dirToA = a->GetOwner()->GetTransform().GetPosition() - (b ? b->GetOwner()->GetTransform().GetPosition() : glm::vec3(0));
		if (glm::dot(dirToA, normal) < 0) {
			normal = -normal;
		}

		float penetration = glm::length(overlap);
		glm::vec3 correction = normal * penetration;

		if (!a->IsStatic()) {
			glm::vec3 posA = a->GetOwner()->GetTransform().GetPosition();
			a->GetOwner()->GetTransform().SetPosition(posA + correction * (invMassA / totalInvMass));
		}

		if (b && !b->IsStatic()) {
			glm::vec3 posB = b->GetOwner()->GetTransform().GetPosition();
			b->GetOwner()->GetTransform().SetPosition(posB - correction * (invMassB / totalInvMass));
		}

		if (!a->IsStatic())
		{
			a->SetVelocity(glm::vec3(0.0f));
			a->SetAngularVelocity(glm::vec3(0.0f));
		}

		if (b && !b->IsStatic())
		{
			b->SetVelocity(glm::vec3(0.0f));
			b->SetAngularVelocity(glm::vec3(0.0f));
		}
	}

	void Scene::OnRender(std::shared_ptr<Camera> camera)
	{
		std::vector<std::shared_ptr<Lights::PointLight>> lights;

		for (auto& e : m_Entities)
		{
			if (auto l = std::dynamic_pointer_cast<Lights::PointLight>(e))
				if (l->IsEnabled()) lights.push_back(l);
		}

		Engine::Rendering::Renderer::BeginScene(camera, *this);

		for (auto& e : m_Entities)
		{
			if (auto obj = std::dynamic_pointer_cast<GameObject>(e))
				if (obj->IsEnabled()) obj->Draw();
		}

		Engine::Rendering::Renderer::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_SceneCamera->SetViewportSize(width, height);
	}
}