#pragma once

#include "Engine/Framework/Component.h"
#include "Engine/Framework/MeshLibrary.h"

#include <glm/glm.hpp>

namespace Engine::Framework::Physics
{
    class Collider : public Component
    {
    public:
        Collider(Entity* owner) : Component(owner) {}
        ComponentType GetType() const override { return ComponentType::Collider; }

		virtual ~Collider() = default;

		void SetStatic(bool isStatic) { m_IsStatic = isStatic; }
        bool IsStatic() const { return m_IsStatic; }
		
		void SetTrigger(bool isTrigger) { m_IsTrigger = isTrigger; }
		bool IsTrigger() const { return m_IsTrigger; }

        // FOR RAYCASTING
        static bool IntersectsOBB(const glm::vec3& rayOrigin, const glm::vec3& rayDir, Entity& entity, float& outDist);
        static bool IntersectsSphere(const glm::vec3& rayOrigin, const glm::vec3& rayDir, Entity& entity, float radius, float& outDist);

        virtual bool CheckCollision(Collider* other, glm::vec3& outOverlap) = 0;
        virtual bool CheckCollision(Collider* other, glm::vec3& outOverlap, glm::vec3& outHitPoint) = 0;
    protected:
        bool m_IsStatic = false;
		bool m_IsTrigger = false;
    };

    class SphereCollider : public Collider 
    {
    public:
        SphereCollider(Entity* owner, float radius = 0.5f) : Collider(owner), m_Radius(radius) {}

        bool CheckCollision(Collider* other, glm::vec3& outOverlap) override;
        float GetRadius() const { return m_Radius; }
    private:
        float m_Radius;
    };

    class CubeCollider : public Collider 
    {
    public:
        CubeCollider(Entity* owner, glm::vec3 size = glm::vec3(1.0f)) : Collider(owner), m_Size(size) 
        {
			m_VerticesCache = Engine::Framework::MeshLibrary::GetCPUMeshCube().vertices;
        }

		bool CheckCollision(Collider* other, glm::vec3& outOverlap) override; // collision without bounce
        bool CheckCollision(Collider* other, glm::vec3& outOverlap, glm::vec3& outHitPoint) override;
        
        glm::vec3 GetSize() const { return m_Size; }
    private:
        glm::vec3 m_Size;
    private:
        std::vector<Engine::Framework::Geometry::Vertex> m_VerticesCache;
    };
}