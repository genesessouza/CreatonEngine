#pragma once

#include "Engine/Framework/Component.h"

namespace Engine::Framework::Physics
{
	class PhysicsComponent : public Component
	{
	public:
		PhysicsComponent(Entity* owner) : Component(owner) {}
		virtual ~PhysicsComponent() = default;

		ComponentType GetType() const override { return ComponentType::Physics; }

		void SetMass(float mass) { m_Mass = (mass <= 0.0f) ? 0.1f : mass; }
		float GetMass() const { return m_Mass; }

		float GetInverseMass() const { return m_IsStatic ? 0.0f : 1.0f / m_Mass; }

		void Update(float ts)
		{
			if (m_IsStatic) return;

			auto& t = m_Owner->GetTransform();
			t.SetPosition(t.GetPosition() + m_Velocity * ts);

			glm::vec3 rot = t.GetRotation();
			rot += glm::degrees(m_AngularVelocity * ts);
			t.SetRotation(rot);

			m_Velocity *= 0.98f;
			m_AngularVelocity *= 0.96f;
		}

		void ApplyGravity(float ts)
		{
			m_Velocity.y += m_Gravity * ts;
		}

		void ApplyImpulse(const glm::vec3& impulse, const glm::vec3& contactPoint)
		{
			if (m_IsStatic) return;

			m_Velocity += impulse * GetInverseMass();

			glm::vec3 r = contactPoint - m_Owner->GetTransform().GetPosition();
			glm::vec3 torque = glm::cross(r, impulse);

			m_AngularVelocity += torque * GetInverseMass() * 2.0f;
		}

		glm::vec3 GetVelocity() const { return m_Velocity; }
		void SetVelocity(const glm::vec3& v) { m_Velocity = v; }

		glm::vec3 GetAngularVelocity() const { return m_AngularVelocity; }
		void SetAngularVelocity(const glm::vec3& av) { m_AngularVelocity = av; }

		void SetStatic(bool s) { m_IsStatic = s; }
		bool IsStatic() const { return m_IsStatic; }
	private:
		float m_Mass = 1.0f;

		glm::vec3 m_Velocity{ 0.0f };
		glm::vec3 m_AngularVelocity{ 0.0f };

		const float m_Gravity = -9.81f;

		bool m_IsStatic = false;
	};
}