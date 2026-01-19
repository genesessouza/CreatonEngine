#pragma once

#include "Engine/Framework/Transform.h"

#include <glm/ext/matrix_clip_space.hpp>

#include <string>
#include <memory>

namespace Engine::Framework
{
    class Camera
    {
    public:
        Camera();

        const glm::mat4 GetViewMatrix();
        const glm::mat4 GetProjectionMatrix();
        const glm::mat4 GetViewProjectionMatrix() const;

        void SetViewportSize(uint32_t width, uint32_t height);
        
        const void SetFOV(float newFOV) { m_FOV = newFOV; }
        
        const void SetNear(float newNearPlane) { m_NearPlane = newNearPlane; }
        const void SetFar(float newFarPlane) { m_FarPlane = newFarPlane; }

        const void MoveCamera(const glm::vec3& position);
        const void RotateCamera(const glm::vec3& rotation);
    
        const glm::vec3& GetPosition() const { return m_Transform.GetPosition(); }

        static std::shared_ptr<Camera> Create() { return std::make_shared<Camera>(); }
    private:
        float m_FOV = 60.0f;
        float m_AspectRatio = 16.0f / 9.0f;
        float m_NearPlane = 0.1f;
        float m_FarPlane = 100.0f;

        bool m_Dirty = false;
    private:
        Engine::Framework::Transform m_Transform;

        glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
        glm::mat4 m_CachedViewMatrix = glm::mat4(1.0f);
    };
}