#include "Engine/Framework/Physics/Collider.h"

namespace Engine::Framework::Physics
{
	// RAYCASTING
	bool Collider::IntersectsOBB(const glm::vec3& rayOrigin, const glm::vec3& rayDir, Entity& entity, float& outDist)
	{
		glm::mat4 modelMatrix = entity.GetTransform().GetMatrix();
		glm::mat4 invModel = glm::inverse(modelMatrix);

		glm::vec3 localRayOrigin = glm::vec3(invModel * glm::vec4(rayOrigin, 1.0f));
		glm::vec3 localRayDir = glm::normalize(glm::vec3(invModel * glm::vec4(rayDir, 0.0f)));

		glm::vec3 aabbMin(-0.5f);
		glm::vec3 aabbMax(0.5f);

		float tmin = (aabbMin.x - localRayOrigin.x) / localRayDir.x;
		float tmax = (aabbMax.x - localRayOrigin.x) / localRayDir.x;
		if (tmin > tmax) std::swap(tmin, tmax);

		float tymin = (aabbMin.y - localRayOrigin.y) / localRayDir.y;
		float tymax = (aabbMax.y - localRayOrigin.y) / localRayDir.y;
		if (tymin > tymax) std::swap(tymin, tymax);

		if ((tmin > tymax) || (tymin > tmax)) return false;
		if (tymin > tmin) tmin = tymin;
		if (tymax < tmax) tmax = tymax;

		float tzmin = (aabbMin.z - localRayOrigin.z) / localRayDir.z;
		float tzmax = (aabbMax.z - localRayOrigin.z) / localRayDir.z;
		if (tzmin > tzmax) std::swap(tzmin, tzmax);

		if ((tmin > tzmax) || (tzmin > tmax)) return false;
		if (tzmin > tmin) tmin = tzmin;

		if (tmin < 0) return false;

		glm::vec3 hitPointLocal = localRayOrigin + localRayDir * tmin;
		glm::vec3 hitPointWorld = glm::vec3(modelMatrix * glm::vec4(hitPointLocal, 1.0f));
		outDist = glm::distance(rayOrigin, hitPointWorld);

		return true;
	}

	bool Collider::IntersectsSphere(const glm::vec3& rayOrigin, const glm::vec3& rayDir, Entity& entity, float radius, float& outDist)
	{
		glm::vec3 oc = rayOrigin - entity.GetTransform().GetPosition();

		float b = glm::dot(oc, rayDir);
		float c = glm::dot(oc, oc) - radius * radius;
		float h = b * b - c;

		if (h < 0.0f) return false;

		h = sqrt(h);

		outDist = -b - h;
		return outDist >= 0;
	}

	bool SphereCollider::CheckCollision(Collider* other, glm::vec3& outOverlap)
	{
		SphereCollider* sphereOther = dynamic_cast<SphereCollider*>(other);
		if (sphereOther)
		{
			glm::vec3 posA = m_Owner->GetTransform().GetPosition();
			glm::vec3 posB = sphereOther->m_Owner->GetTransform().GetPosition();
			float radiusSum = m_Radius + sphereOther->m_Radius;
			float dist = glm::distance(posA, posB);
			if (dist < radiusSum)
			{
				outOverlap = glm::normalize(posB - posA) * (radiusSum - dist);
				return true;
			}
		}
		return false;
	}

	bool CubeCollider::CheckCollision(Collider* other, glm::vec3& outOverlap)
	{
		return false;
	}

	bool CubeCollider::CheckCollision(Collider* other, glm::vec3& outOverlap, glm::vec3& outHitPoint)
	{
		bool collisionDetected = false;

		if (auto otherCube = dynamic_cast<CubeCollider*>(other))
		{
			Transform& tA = m_Owner->GetTransform();
			Transform& tB = otherCube->m_Owner->GetTransform();

			glm::vec3 distanceVec = tB.GetPosition() - tA.GetPosition();

			float overlapX = (tA.GetScale().x * 0.5f + tB.GetScale().x * 0.5f) - std::abs(distanceVec.x);
			float overlapY = (tA.GetScale().y * 0.5f + tB.GetScale().y * 0.5f) - std::abs(distanceVec.y);
			float overlapZ = (tA.GetScale().z * 0.5f + tB.GetScale().z * 0.5f) - std::abs(distanceVec.z);

			if (overlapX > 0 && overlapY > 0 && overlapZ > 0)
			{
				if (overlapY < overlapX && overlapY < overlapZ)
					outOverlap = glm::vec3(0, (distanceVec.y > 0 ? -overlapY : overlapY), 0);
				else if (overlapX < overlapZ)
					outOverlap = glm::vec3((distanceVec.x > 0 ? -overlapX : overlapX), 0, 0);
				else
					outOverlap = glm::vec3(0, 0, (distanceVec.z > 0 ? -overlapZ : overlapZ));

				collisionDetected = true;
			}
		}
		return collisionDetected;
	}
}