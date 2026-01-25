#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Engine::Framework
{
    class Debugging 
    {
    public:
        struct DebugLine 
        {
            glm::vec3 Start, End;
            glm::vec4 Color;
            float LifeTime;
        };

        struct DebugPoint 
        {
            glm::vec3 Center;
            float Radius;
            glm::vec4 Color;
            float LifeTime;
        };

        static void AddLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, float duration = 2.0f);
        static void AddHitPoint(const glm::vec3& center, float radius, const glm::vec4& color, float duration = 2.0f);

        static void Render(float deltaTime, const glm::mat4& view, const glm::mat4& proj);
    private:
        static std::vector<DebugLine> s_Lines;
        static std::vector<DebugPoint> s_Points;
    };
}