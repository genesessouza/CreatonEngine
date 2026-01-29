#pragma once

#include "Engine/Framework/Component.h"
#include "Engine/Framework/GameObject.h"
#include "Engine/Framework/Physics/Collider.h"

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Engine::Framework::Physics
{
	class PhysicsComponent : public Engine::Framework::Component
	{
	public:
		PhysicsComponent() = default;
		virtual ~PhysicsComponent() = default;

		void SetMass(float mass) { m_Mass = (mass <= 0.0f) ? 0.1f : mass; }
		float GetMass() const { return m_Mass; }
		
		float GetInverseMass() const { return m_IsStatic ? 0.0f : 1.0f / m_Mass; }

		const glm::mat3& GetInverseInertiaTensorWorld() const { return m_InvInertiaTensorWorld; }

		void ApplyGravity(float ts)
		{
			m_Velocity.y += m_Gravity * ts;
		}

		void CalculateInertiaTensor()
		{
			auto coll = m_Owner->GetComponent<Collider>();

			if (IsStatic() || m_Mass == 0.0f)
			{
				m_InvInertiaTensorLocal = glm::mat3(0.0f);
				m_InvInertiaTensorWorld = glm::mat3(0.0f);
				return;
			}

			glm::vec3 halfSize = coll->GetHalfSize();

			float x2 = (halfSize.x * 2.0f) * (halfSize.x * 2.0f);
			float y2 = (halfSize.y * 2.0f) * (halfSize.y * 2.0f);
			float z2 = (halfSize.z * 2.0f) * (halfSize.z * 2.0f);

			float fraction = 1.0f / 12.0f;

			glm::mat3 inertiaTensor(0.0f);
			inertiaTensor[0][0] = fraction * m_Mass * (y2 + z2);
			inertiaTensor[1][1] = fraction * m_Mass * (x2 + z2);
			inertiaTensor[2][2] = fraction * m_Mass * (x2 + y2);

			m_InvInertiaTensorLocal = glm::inverse(inertiaTensor);
		}

		void UpdateInertiaTensorWorld()
		{
			if (IsStatic()) return;

			glm::mat3 R = glm::toMat3(GetOwner()->GetTransform().GetRotationQuat());
			m_InvInertiaTensorWorld = R * m_InvInertiaTensorLocal * glm::transpose(R);
		}

		glm::vec3 GetVelocity() const { return m_Velocity; }
		void SetVelocity(const glm::vec3& v) { m_Velocity = v; }

		glm::vec3 GetAngularVelocity() const { return m_AngularVelocity; }
		void SetAngularVelocity(const glm::vec3& av) { m_AngularVelocity = av; }

		void SetStatic(bool s) { m_IsStatic = s; }
		bool IsStatic() const { return m_IsStatic; }
	private:
		float m_Mass = 1.0f;
		const float m_Gravity = -9.81f;

		glm::mat3 m_InvInertiaTensorLocal = glm::mat3(0.0f);
		glm::mat3 m_InvInertiaTensorWorld;

		glm::vec3 m_Velocity{ 0.0f };
		glm::vec3 m_AngularVelocity{ 0.0f };

		bool m_IsStatic = false;
	};
}