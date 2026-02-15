#pragma once

#include "Engine/Framework/Component.h"

#include <glm/glm.hpp>

namespace Engine::Framework
{
	class Entity;
	class Scene;
}

namespace Engine::Framework::Physics
{
	class Collider : public Component
	{
	public:
		struct AABB
		{
			glm::vec3 Min;
			glm::vec3 Max;
		};
		struct OBB
		{
			glm::vec3 Center;
			glm::vec3 NormalizedAxes[3];
			glm::vec3 HalfSize;
		};
		struct ContactManifold
		{
			bool isColliding = false;
			glm::vec3 normal;
			float depth;
			glm::vec3 contactPointA;
			glm::vec3 contactPointB;
		};
		enum ColliderType
		{
			Sphere,
			Cube
		};
	public:
		Collider()
			: Component(), m_AABB{}, m_OBB{}, m_HalfSize(0.0f), m_ColliderType()
		{
		}

		virtual ~Collider() = default;

		void OnUpdate() override;

		void SetStatic(bool isStatic) { m_IsStatic = isStatic; }
		bool IsStatic() const { return m_IsStatic; }

		void SetTrigger(bool isTrigger) { m_IsTrigger = isTrigger; }
		bool IsTrigger() const { return m_IsTrigger; }

		bool MovedThisFrame() const;

		const AABB& GetAABB()
		{
			UpdateCacheIfNeeded();
			return m_AABB;
		}

		virtual OBB& GetOBB()
		{
			UpdateCacheIfNeeded();
			return m_OBB;
		}

		// FOR RAYCASTING
		static bool IntersectsOBB(const glm::vec3& rayOrigin, const glm::vec3& rayDir, Entity& entity, float& outDist);
		static bool IntersectsSphere(const glm::vec3& rayOrigin, const glm::vec3& rayDir, Entity& entity, float radius, float& outDist);

		inline virtual glm::vec3 GetHalfSize() const { return m_HalfSize; }

		virtual bool CheckCollision(Collider* other, glm::vec3& outOverlap, ContactManifold& manifold)
		{
			if (m_ColliderType == Sphere)
				return CheckCollisionImpl(other, outOverlap);
			else
				return CheckCollisionManifoldImpl(other, manifold);
		}
	protected:
		virtual bool CheckCollisionImpl(Collider* other, glm::vec3& outOverlap) = 0;
		virtual bool CheckCollisionManifoldImpl(Collider* other, ContactManifold& outManifold) = 0;
		virtual void RebuildCache() {}

		void UpdateCacheIfNeeded()
		{
			if (!m_CacheDirty && !MovedThisFrame())
				return;

			RebuildCache();
			m_CacheDirty = false;
		}
	protected:
		bool m_IsStatic = false;
		bool m_IsTrigger = false;

		bool m_CacheDirty = true;

		ColliderType m_ColliderType;
		glm::vec3 m_HalfSize;

		AABB m_AABB;
		OBB m_OBB;
	};

	class SphereCollider : public Collider
	{
	public:
		SphereCollider(float radius = 0.5f)
			: Collider()
		{
			m_ColliderType = ColliderType::Sphere;
			m_Radius = radius;
		}

		virtual ~SphereCollider() = default;

		void SetRadius(float radius) { m_Radius = radius; }
		float GetRadius() const { return m_Radius; }

	protected:
		bool CheckCollisionImpl(Collider* other, glm::vec3& outOverlap) override;
		bool CheckCollisionManifoldImpl(Collider* other, ContactManifold& outOverlap) override { return false; }
		void RebuildCache() override;
	private:
		float m_Radius;
	};

	class CubeCollider : public Collider
	{
	public:
		CubeCollider(const glm::vec3& size = glm::vec3(1.0f))
			: Collider(), m_Size(size)
		{
			m_ColliderType = ColliderType::Cube;
			m_HalfSize = size * 0.5f;
		}

		virtual ~CubeCollider() = default;

		void SetSize(const glm::vec3& size) { m_Size = size; }
		glm::vec3 GetSize() const { return m_Size; }

		float ProjectOBB(const OBB& obb, const glm::vec3& axis);

		bool SimpleSAT(const OBB& collider, const OBB& other);
		bool FullSAT(const OBB& collider, const OBB& other, ContactManifold& manifold);
	protected:
		glm::vec3 GetSupportPoint(const OBB& obb, const glm::vec3& direction);

		bool CheckCollisionImpl(Collider* other, glm::vec3& outOverlap) override { return false; }
		bool CheckCollisionManifoldImpl(Collider* other, ContactManifold& outManifold) override;
		void RebuildCache() override;

	private:
		glm::vec3 m_Size;
	};
}