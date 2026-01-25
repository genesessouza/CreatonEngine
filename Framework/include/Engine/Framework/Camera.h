#pragma once

#include "Engine/Framework/Entity.h"

#include <glm/ext/matrix_clip_space.hpp>

#include <string>
#include <memory>

namespace Engine::Framework
{
    class Camera : public Entity
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

        bool IsDirty() const { return m_Transform.IsDirty(); }
        void ClearDirty() { m_Transform.ClearDirty(); }

        static std::shared_ptr<Camera> Create() { return std::make_shared<Camera>(); }
    private:
        float m_FOV;
        float m_AspectRatio;
        float m_NearPlane;
        float m_FarPlane;
    private:
        glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
        glm::mat4 m_CachedViewMatrix = glm::mat4(1.0f);
    };
}