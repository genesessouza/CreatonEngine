#pragma once

#include <glm/glm.hpp>

#include <glm/gtc/quaternion.hpp>

namespace Engine::Framework
{
	class Transform
	{
    public:
        Transform() = default;
        virtual ~Transform() = default;
        
        const glm::mat4& GetWorldMatrix() const;

        void SetPosition(const glm::vec3& pos) { m_Position = pos; MarkDirty(); }
        void SetRotation(const glm::quat& rot) { m_Rotation = rot; MarkDirty(); }
        void SetScale(const glm::vec3& scale) { m_Scale = scale; MarkDirty(); }

        const glm::vec3& GetPosition() const { return m_Position; }
        const glm::vec3& GetRotation() const { return glm::eulerAngles(m_Rotation); }
        const glm::quat& GetRotationQuat() const { return m_Rotation; }
        const glm::vec3& GetScale() const { return m_Scale; }

        bool WasModifiedThisFrame() const { return m_ModifiedThisFrame; }
        void ClearFrameFlag() { m_ModifiedThisFrame = false; }
    private:
        void MarkDirty();
        void Recalculate() const;
    private:
        glm::vec3 m_Position;
        glm::quat m_Rotation;
        glm::vec3 m_Scale;

        mutable glm::mat4 m_WorldMatrix;

        bool m_Dirty = true;
        bool m_ModifiedThisFrame = false;
    };
}