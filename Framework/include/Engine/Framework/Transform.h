#pragma once

#include <glm/glm.hpp>

namespace Engine::Framework
{
    class Transform
    {
    public:
        Transform() = default;
        virtual ~Transform() = default;

        void SetPosition(const glm::vec3& position) { m_Position = position; }
        void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; }
        void SetScale(const glm::vec3& scale) { m_Scale = scale; }

        const glm::vec3& GetPosition() const { return m_Position; }
        const glm::vec3& GetRotation() const { return m_Rotation; }
        const glm::vec3& GetScale() const { return m_Scale; }

        glm::mat4 GetMatrix() const;
    private:
        glm::vec3 m_Position{ 0, 0, 0 };
        glm::vec3 m_Rotation{ 0, 0, 0 };
        glm::vec3 m_Scale{ 1, 1, 1 };
    };
}