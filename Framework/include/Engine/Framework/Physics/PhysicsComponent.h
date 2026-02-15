#pragma once

#include "Engine/Framework/Component.h"
#include "Engine/Framework/Physics/Gravity.h"

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <iostream>

namespace Engine::Framework
{
	class Entity;
}
namespace Engine::Framework::Physics
{
	class Collider;

	class PhysicsComponent : public Component
	{
	public:
		PhysicsComponent() : Component(), m_InvInertiaTensorWorld(0.0f) {}
		virtual ~PhysicsComponent() = default;

		void SetMass(float mass) { m_Mass = (mass <= 0.0f) ? 0.1f : mass; }
		float GetMass() const { return m_Mass; }
		
		float GetInverseMass() const { return m_IsStatic ? 0.0f : 1.0f / m_Mass; }

		const glm::mat3& GetInverseInertiaTensorWorld() const { return m_InvInertiaTensorWorld; }

		void ApplyGravity(float ts)
		{
			m_Velocity.y += Gravity::GravityAcceleration().y * ts;
		}

		void CalculateInertiaTensor();
		void UpdateInertiaTensorWorld();

		glm::vec3 GetVelocity() const { return m_Velocity; }
		void SetVelocity(const glm::vec3& v) { m_Velocity = v; }

		glm::vec3 GetAngularVelocity() const { return m_AngularVelocity; }
		void SetAngularVelocity(const glm::vec3& av) { m_AngularVelocity = av; }

		void SetStatic(bool s) { m_IsStatic = s; }
		bool IsStatic() const { return m_IsStatic; }
	private:
		float m_Mass = 1.0f;

		glm::mat3 m_InvInertiaTensorLocal = glm::mat3(0.0f);
		glm::mat3 m_InvInertiaTensorWorld;

		glm::vec3 m_Velocity{ 0.0f };
		glm::vec3 m_AngularVelocity{ 0.0f };

		bool m_IsStatic = false;
	};
}