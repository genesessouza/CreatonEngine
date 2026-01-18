#pragma once

#include <glm/glm.hpp>

namespace Engine::Framework
{
    class Transform
    {
    public:
        glm::mat4 GetMatrix() const;
    public:
        glm::vec3 m_Position{ 0,0,0 };
        glm::vec3 m_Rotation{ 0,0,0 }; // Euler por enquanto
        glm::vec3 m_Scale{ 1,1,1 };
    };
}