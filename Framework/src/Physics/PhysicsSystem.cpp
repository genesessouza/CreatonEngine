#include "Engine/Framework/Physics/PhysicsSystem.h"

#include "Engine/Framework/GameObject.h"

namespace Engine::Framework::Physics
{
	bool PhysicsSystem::AABBOverlap(const Collider::AABB& a, const Collider::AABB& b)
	{
		if (a.Max.x < b.Min.x || a.Min.x > b.Max.x) return false;
		if (a.Max.y < b.Min.y || a.Min.y > b.Max.y) return false;
		if (a.Max.z < b.Min.z || a.Min.z > b.Max.z) return false;

		return true;
	}

	void PhysicsSystem::UpdateCaches(const Scene& scene)
	{
		auto& colliders = scene.GetColliders();

		m_SceneColliders.clear();
		m_SceneColliders.reserve(colliders.size());
		for (auto* coll : colliders)
			m_SceneColliders.push_back(coll);

		auto& physicsComps = scene.GetPhysicsComponents();

		m_ScenePhysicsComps.clear();
		m_ScenePhysicsComps.reserve(physicsComps.size());
		for (auto* phys : physicsComps)
		{
			phys->CalculateInertiaTensor();
			m_ScenePhysicsComps.push_back(phys);
		}
	}

	void PhysicsSystem::Step(float dt)
	{
		for (PhysicsComponent* phys : m_ScenePhysicsComps)
		{
			if (!phys->GetOwner()->IsEnabled() || !phys->IsEnabled() || phys->IsStatic()) continue;

			//phys->UpdateInertiaTensorWorld();

			auto collider = phys->GetOwner()->GetComponent<Collider>();
			if (collider)
			{
				auto& obb = collider->GetOBB();
				glm::mat3 R = glm::toMat3(phys->GetOwner()->GetTransform().GetOrientation());
				obb.NormalizedAxes[0] = R[0];
				obb.NormalizedAxes[1] = R[1];
				obb.NormalizedAxes[2] = R[2];
				obb.Center = phys->GetOwner()->GetTransform().GetPosition();
			}

			phys->ApplyGravity(dt);
			phys->SetAngularVelocity(phys->GetAngularVelocity() * 0.98f);
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
					ResolveCollision(b->GetOwner()->GetComponent<PhysicsComponent>(), a->GetOwner()->GetComponent<PhysicsComponent>(), manifold);
			}
		}

		for (PhysicsComponent* phys : m_ScenePhysicsComps)
		{
			if (!phys->GetOwner()->IsEnabled() || !phys->IsEnabled() || phys->IsStatic()) continue;

			auto& transform = phys->GetOwner()->GetTransform();
			transform.Translate(phys->GetVelocity() * dt, false);

			glm::vec3 w = phys->GetAngularVelocity();
			glm::quat q = transform.GetOrientation();
			glm::quat q_dot = 0.5f * glm::quat(0, w.x, w.y, w.z) * q;
			glm::quat next_q = q + q_dot * dt;
			transform.Rotate(glm::normalize(glm::degrees(glm::eulerAngles(next_q))), true);
		}
	}

	void PhysicsSystem::ResolveCollision(PhysicsComponent* a, PhysicsComponent* b, Collider::ContactManifold& m)
	{
		const float percent = 0.8f;
		const float slop = 0.01f;
		glm::vec3 correction = m.normal * std::max(m.depth - slop, 0.0f) / (a->GetInverseMass() + b->GetInverseMass()) * percent;

		if (!a->IsStatic()) a->GetOwner()->GetTransform().SetPosition(a->GetOwner()->GetTransform().GetPosition() - correction * a->GetInverseMass());
		if (!b->IsStatic()) b->GetOwner()->GetTransform().SetPosition(b->GetOwner()->GetTransform().GetPosition() + correction * b->GetInverseMass());

		glm::vec3 rA = m.contactPointA - a->GetOwner()->GetTransform().GetPosition();
		glm::vec3 rB = m.contactPointB - b->GetOwner()->GetTransform().GetPosition();

		glm::vec3 velA = a->GetVelocity() + glm::cross(a->GetAngularVelocity(), rA);
		glm::vec3 velB = b->GetVelocity() + glm::cross(b->GetAngularVelocity(), rB);
		glm::vec3 relativeVel = velB - velA;

		float velAlongNormal = glm::dot(relativeVel, m.normal);
		if (velAlongNormal > 0) return;

		float e = 0.7f;
		float raxn = glm::dot(glm::cross(rA, m.normal), a->GetInverseInertiaTensorWorld() * glm::cross(rA, m.normal));
		float rbxn = glm::dot(glm::cross(rB, m.normal), b->GetInverseInertiaTensorWorld() * glm::cross(rB, m.normal));

		float j = -(1.0f + e) * velAlongNormal;
		j /= (a->GetInverseMass() + b->GetInverseMass() + raxn + rbxn);

		glm::vec3 impulse = m.normal * j;

		glm::vec3 torqueA = glm::cross(rA, impulse);
		glm::vec3 deltaAngularVel = a->GetInverseInertiaTensorWorld() * torqueA;

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
}