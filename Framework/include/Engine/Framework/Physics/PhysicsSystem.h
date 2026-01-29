#pragma once

#include "Engine/Framework/Scene.h"

#include "Engine/Framework/Physics/Collider.h"
#include "Engine/Framework/Physics/Gravity.h"
#include "Engine/Framework/Physics/PhysicsComponent.h"

#include <glm/glm.hpp>
#include <iostream>

namespace Engine::Framework::Physics
{
	class PhysicsSystem
	{
	public:
		PhysicsSystem() = default;
		virtual ~PhysicsSystem() = default;

		static bool AABBOverlap(const Collider::AABB& a, const Collider::AABB& b)
		{
			if (a.Max.x < b.Min.x || a.Min.x > b.Max.x) return false;
			if (a.Max.y < b.Min.y || a.Min.y > b.Max.y) return false;
			if (a.Max.z < b.Min.z || a.Min.z > b.Max.z) return false;

			return true;
		}

		static void UpdateCaches(const Scene& scene)
		{
			auto& colliders = scene.GetColliders();
			if (m_SceneColliders.size() != colliders.size())
			{
				m_SceneColliders.clear();
				m_SceneColliders.reserve(colliders.size());
				for (auto* coll : colliders)
					m_SceneColliders.push_back(coll);
			}

			auto& physicsComps = scene.GetPhysicsComponents();
			if (m_ScenePhysicsComps.size() != physicsComps.size())
			{
				m_ScenePhysicsComps.clear();
				m_ScenePhysicsComps.reserve(physicsComps.size());
				for (auto* phys : physicsComps)
				{
					phys->CalculateInertiaTensor();
					m_ScenePhysicsComps.push_back(phys);
				}
			}
		}

		static void Step(float dt)
		{
			for (PhysicsComponent* phys : m_ScenePhysicsComps)
			{
				if (!phys->GetOwner()->IsEnabled() || phys->IsStatic()) continue;

				phys->ApplyGravity(dt);

				phys->SetVelocity(phys->GetVelocity());
				phys->SetAngularVelocity(phys->GetAngularVelocity() * 0.99f);
			}

			for (size_t i = 0; i < m_SceneColliders.size(); ++i)
			{
				for (size_t j = i + 1; j < m_SceneColliders.size(); ++j)
				{
					Collider* a = m_SceneColliders[i];
					Collider* b = m_SceneColliders[j];

					if (a->IsStatic() && b->IsStatic()) continue;
					if (!AABBOverlap(a->GetAABB(), b->GetAABB())) continue;

					Collider::ContactManifold manifold;
					
					glm::vec3 overlap;
					if (a->CheckCollision(b, overlap, manifold))
					{
						auto aPhys = a->GetOwner()->GetComponent<PhysicsComponent>();
						ResolveCollision(a->GetOwner()->GetComponent<PhysicsComponent>(), b->GetOwner()->GetComponent<PhysicsComponent>(), manifold);

						std::cout << "\n--- COLISÃO DETECTADA ---" << std::endl;
						std::cout << "Profundidade (Depth): " << manifold.depth << std::endl;
						std::cout << "Normal (A->B): " << manifold.normal.x << ", " << manifold.normal.y << ", " << manifold.normal.z << std::endl;
						std::cout << "Ponto em A (Vértice): " << manifold.contactPointA.x << ", " << manifold.contactPointA.y << ", " << manifold.contactPointA.z << std::endl;
						std::cout << "Ponto em B (Chão): " << manifold.contactPointB.x << ", " << manifold.contactPointB.y << ", " << manifold.contactPointB.z << std::endl;

							std::cout << "Velocidade Angular A: " << aPhys->GetAngularVelocity().x << ", "
								<< aPhys->GetAngularVelocity().y << ", " << aPhys->GetAngularVelocity().z << std::endl;
						std::cout << "-------------------------\n" << std::endl;
					}
				}
			}

			for (PhysicsComponent* phys : m_ScenePhysicsComps)
			{
				if (!phys->GetOwner()->IsEnabled() || phys->IsStatic()) continue;

				auto& transform = phys->GetOwner()->GetTransform();
				auto collider = phys->GetOwner()->GetComponent<Collider>();

				if (collider)
				{
					// 1. Pega a OBB (garanta que você tenha um método GetOBB() ou acesso a ela)
					Collider::OBB& obb = collider->GetOBB();

					// 2. Extrai a matriz de rotação 3x3 do Quaternion de rotação do Transform
					glm::mat3 R = glm::toMat3(transform.GetRotationQuat());

					// 3. Atualiza os eixos: as colunas da matriz de rotação SÃO os eixos locais no mundo
					obb.NormalizedAxes[0] = glm::normalize(R[0]); // Eixo X local
					obb.NormalizedAxes[1] = glm::normalize(R[1]); // Eixo Y local
					obb.NormalizedAxes[2] = glm::normalize(R[2]); // Eixo Z local

					// 4. Não esqueça de atualizar o centro também!
					obb.Center = transform.GetPosition();
				}
				transform.SetPosition(transform.GetPosition() + phys->GetVelocity() * dt);

				glm::vec3 w = phys->GetAngularVelocity();
				glm::quat q = transform.GetRotationQuat();
				glm::quat q_dot = 0.5f * glm::quat(0, w.x, w.y, w.z) * q;
				glm::quat next_q = q + q_dot * dt;

				transform.SetRotation(glm::normalize(next_q));

				phys->UpdateInertiaTensorWorld();
			}
		}

		static void ResolveCollision(PhysicsComponent* a, PhysicsComponent* b, Collider::ContactManifold& m)
		{
			const float percent = 0.8f;
			const float slop = 0.01f;
			glm::vec3 correction = m.normal * std::max(m.depth - slop, 0.0f) / (a->GetInverseMass() + b->GetInverseMass()) * percent;

			if (!a->IsStatic()) a->GetOwner()->GetTransform().SetPosition(a->GetOwner()->GetTransform().GetPosition() - correction * a->GetInverseMass());
			if (!b->IsStatic()) b->GetOwner()->GetTransform().SetPosition(b->GetOwner()->GetTransform().GetPosition() + correction * b->GetInverseMass());

			glm::vec3 rA = m.contactPointB - a->GetOwner()->GetTransform().GetPosition();
			glm::vec3 rB = m.contactPointB - b->GetOwner()->GetTransform().GetPosition();

			glm::vec3 velA = a->GetVelocity() + glm::cross(a->GetAngularVelocity(), rA);
			glm::vec3 velB = b->GetVelocity() + glm::cross(b->GetAngularVelocity(), rB);
			glm::vec3 relativeVel = velB - velA;

			float velAlongNormal = glm::dot(relativeVel, m.normal);
			//if (velAlongNormal > 0) return;

			float e = 0.7f;
			float raxn = glm::dot(glm::cross(rA, m.normal), a->GetInverseInertiaTensorWorld() * glm::cross(rA, m.normal));
			float rbxn = glm::dot(glm::cross(rB, m.normal), b->GetInverseInertiaTensorWorld() * glm::cross(rB, m.normal));

			float j = -(1.0f + e) * velAlongNormal;
			j /= (a->GetInverseMass() + b->GetInverseMass() + raxn + rbxn);

			glm::vec3 impulse = m.normal * j;

			if (!a->IsStatic())
			{
				a->SetVelocity(a->GetVelocity() - impulse * a->GetInverseMass());
				a->SetAngularVelocity(a->GetAngularVelocity() - a->GetInverseInertiaTensorWorld() * glm::cross(rA, impulse));
			}
			if (!b->IsStatic())
			{
				b->SetVelocity(b->GetVelocity() + impulse * b->GetInverseMass());
				b->SetAngularVelocity(b->GetAngularVelocity() + b->GetInverseInertiaTensorWorld() * glm::cross(rB, impulse));
			}

			velA = a->GetVelocity() + glm::cross(a->GetAngularVelocity(), rA);
			velB = b->GetVelocity() + glm::cross(b->GetAngularVelocity(), rB);
			relativeVel = velB - velA;

			glm::vec3 tangent = relativeVel - (m.normal * glm::dot(relativeVel, m.normal));

			if (glm::length(tangent) > 0.001f)
			{
				tangent = glm::normalize(tangent);

				float raxt = glm::dot(glm::cross(rA, tangent), a->GetInverseInertiaTensorWorld() * glm::cross(rA, tangent));
				float rbxt = glm::dot(glm::cross(rB, tangent), b->GetInverseInertiaTensorWorld() * glm::cross(rB, tangent));

				float jt = -glm::dot(relativeVel, tangent);
				jt /= (a->GetInverseMass() + b->GetInverseMass() + raxt + rbxt);

				float mu = 0.3f;
				float frictionLimit = j * mu;
				jt = glm::clamp(jt, -frictionLimit, frictionLimit);

				glm::vec3 frictionImpulse = tangent * jt;

				if (!a->IsStatic())
				{
					a->SetVelocity(a->GetVelocity() - frictionImpulse * a->GetInverseMass());
					a->SetAngularVelocity(a->GetAngularVelocity() - a->GetInverseInertiaTensorWorld() * glm::cross(rA, frictionImpulse));
				}
				if (!b->IsStatic())
				{
					b->SetVelocity(b->GetVelocity() + frictionImpulse * b->GetInverseMass());
					b->SetAngularVelocity(b->GetAngularVelocity() + b->GetInverseInertiaTensorWorld() * glm::cross(rB, frictionImpulse));
				}
			}
		}
	private:
		inline static std::vector<Collider*> m_SceneColliders;
		inline static std::vector<PhysicsComponent*> m_ScenePhysicsComps;
	};
}