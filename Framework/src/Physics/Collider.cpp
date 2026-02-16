#include "Engine/Framework/Physics/Collider.h"

#include "Engine/Framework/Entity.h"
#include "Engine/Framework/Scene.h"

namespace Engine::Framework::Physics
{
	void Collider::OnUpdate()
	{
		glm::mat3 R = glm::mat3(GetOwner()->GetTransform().GetOrientation());
		m_OBB.NormalizedAxes[0] = R[0];
		m_OBB.NormalizedAxes[1] = R[1];
		m_OBB.NormalizedAxes[2] = R[2];
		m_OBB.Center = m_Owner->GetTransform().GetPosition();
	
		UpdateCacheIfNeeded();
	}

	bool Collider::MovedThisFrame() const
	{
		return m_Owner->GetTransform().WasModifiedThisFrame();
	}

	// RAYCASTING
	bool Collider::IntersectsOBB(const glm::vec3& rayOrigin, const glm::vec3& rayDir, Entity& entity, float& outDist)
	{
		glm::mat4 modelMatrix = entity.GetTransform().GetWorldMatrix();
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



	// ----------------------------------------------------------------------------------------------- //
	// ----------------------------------------------------------------------------------------------- //
	// ----------------------------------------------------------------------------------------------- //
	// ------------------------------------- SPHERE COLLIDER ----------------------------------------- //
	// ----------------------------------------------------------------------------------------------- //
	// ----------------------------------------------------------------------------------------------- //
	// ----------------------------------------------------------------------------------------------- //

	bool SphereCollider::CheckCollisionImpl(Collider* other, glm::vec3& outOverlap)
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

	void SphereCollider::RebuildCache()
	{
		const auto& pos = m_Owner->GetTransform().GetPosition();

		m_AABB.Min = pos - glm::vec3(m_Radius);
		m_AABB.Max = pos + glm::vec3(m_Radius);
	}



	// ----------------------------------------------------------------------------------------------- //
	// ----------------------------------------------------------------------------------------------- //
	// ----------------------------------------------------------------------------------------------- //
	// ------------------------------------- CUBE COLLIDER ------------------------------------------- //
	// ----------------------------------------------------------------------------------------------- //
	// ----------------------------------------------------------------------------------------------- //
	// ----------------------------------------------------------------------------------------------- //

	float CubeCollider::ProjectOBB(const OBB& obb, const glm::vec3& axis)
	{
		return 
			std::abs(glm::dot(obb.NormalizedAxes[0] * obb.HalfSize.x, axis)) +
			std::abs(glm::dot(obb.NormalizedAxes[1] * obb.HalfSize.y, axis)) +
			std::abs(glm::dot(obb.NormalizedAxes[2] * obb.HalfSize.z, axis));
	}

	bool CubeCollider::SimpleSAT(const OBB& a, const OBB& b)
	{
		glm::vec3 axes[6] =
		{
			a.NormalizedAxes[0], a.NormalizedAxes[1], a.NormalizedAxes[2],
			b.NormalizedAxes[0], b.NormalizedAxes[1], b.NormalizedAxes[2]
		};

		glm::vec3 delta = b.Center - a.Center;

		for (auto& axis : axes)
		{
			float len2 = glm::dot(axis, axis);
			if (len2 < 1e-6f)
				continue;

			glm::vec3 n = axis / std::sqrt(len2);

			float d = std::abs(glm::dot(delta, n));
			float ra = ProjectOBB(a, n);
			float rb = ProjectOBB(b, n);

			if (d > ra + rb)
				return false;
		}

		return true;
	}

	bool CubeCollider::FullSAT(const OBB& a, const OBB& b, ContactManifold& manifold)
	{
		manifold.isColliding = false;
		manifold.depth = FLT_MAX;
		glm::vec3 delta = b.Center - a.Center;

		glm::vec3 axes[15] =
		{
			a.NormalizedAxes[0], a.NormalizedAxes[1], a.NormalizedAxes[2],
			b.NormalizedAxes[0], b.NormalizedAxes[1], b.NormalizedAxes[2],

			glm::cross(a.NormalizedAxes[0], b.NormalizedAxes[0]),
			glm::cross(a.NormalizedAxes[0], b.NormalizedAxes[1]),
			glm::cross(a.NormalizedAxes[0], b.NormalizedAxes[2]),
			glm::cross(a.NormalizedAxes[1], b.NormalizedAxes[0]),
			glm::cross(a.NormalizedAxes[1], b.NormalizedAxes[1]),
			glm::cross(a.NormalizedAxes[1], b.NormalizedAxes[2]),
			glm::cross(a.NormalizedAxes[2], b.NormalizedAxes[0]),
			glm::cross(a.NormalizedAxes[2], b.NormalizedAxes[1]),
			glm::cross(a.NormalizedAxes[2], b.NormalizedAxes[2])
		};

		for (int i = 0; i < 15; i++)
		{
			glm::vec3 n = axes[i];
			float len2 = glm::dot(n, n);
			if (len2 < 1e-6f) continue;
			n = n / std::sqrt(len2);

			float ra = ProjectOBB(a, n);
			float rb = ProjectOBB(b, n);
			float distCentros = std::abs(glm::dot(delta, n));

			float overlap = (ra + rb) - distCentros;

			if (overlap <= 0.0f) 
				return false;

			if (overlap < manifold.depth)
			{
				manifold.depth = overlap;
				manifold.normal = n;
			}
		}

		manifold.isColliding = true;
							
		if (glm::dot(manifold.normal, delta) < 0.0f) {
			manifold.normal = -manifold.normal;
		}

		manifold.contactPointA = GetSupportPoint(a, manifold.normal);
		manifold.contactPointB = manifold.contactPointA - (manifold.normal * manifold.depth);

		return true;
	}

	glm::vec3 CubeCollider::GetSupportPoint(const OBB& obb, const glm::vec3& worldDirection)
	{
		glm::vec3 localDir(
			glm::dot(worldDirection, obb.NormalizedAxes[0]),
			glm::dot(worldDirection, obb.NormalizedAxes[1]),
			glm::dot(worldDirection, obb.NormalizedAxes[2])
		);

		glm::vec3 localContact(
			(localDir.x > 0) ? obb.HalfSize.x : -obb.HalfSize.x,
			(localDir.y > 0) ? obb.HalfSize.y : -obb.HalfSize.y,
			(localDir.z > 0) ? obb.HalfSize.z : -obb.HalfSize.z
		);

		return obb.Center +
			(obb.NormalizedAxes[0] * localContact.x) +
			(obb.NormalizedAxes[1] * localContact.y) +
			(obb.NormalizedAxes[2] * localContact.z);
	}

	bool CubeCollider::CheckCollisionManifoldImpl(Collider* other, ContactManifold& outManifold)
	{
		auto* cube = dynamic_cast<CubeCollider*>(other);
		if (!cube) return false;

		UpdateCacheIfNeeded();
		cube->UpdateCacheIfNeeded();

		return FullSAT(m_OBB, cube->m_OBB, outManifold);
	}

	void CubeCollider::RebuildCache()
	{
		const Transform& t = m_Owner->GetTransform();

		const glm::mat4& M = const_cast<Transform&>(t).GetWorldMatrix();

		m_OBB.Center = glm::vec3(M[3]);

		m_OBB.NormalizedAxes[0] = glm::normalize(glm::vec3(M[0]));
		m_OBB.NormalizedAxes[1] = glm::normalize(glm::vec3(M[1]));
		m_OBB.NormalizedAxes[2] = glm::normalize(glm::vec3(M[2]));

		m_OBB.HalfSize = m_HalfSize;

		// AABB derived from OBB (for broadphase)
		glm::vec3 extents =
			glm::abs(m_OBB.NormalizedAxes[0]) * m_HalfSize.x +
			glm::abs(m_OBB.NormalizedAxes[1]) * m_HalfSize.y +
			glm::abs(m_OBB.NormalizedAxes[2]) * m_HalfSize.z;

		m_AABB.Min = m_OBB.Center - extents;
		m_AABB.Max = m_OBB.Center + extents;
	}
}