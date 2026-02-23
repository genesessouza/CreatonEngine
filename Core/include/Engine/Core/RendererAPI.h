#pragma once

#include <cstdint>
#include <memory>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace Engine::Core
{
    class RendererAPI
    {
    public:
        virtual ~RendererAPI() = default;

        virtual void Clear() = 0;
        virtual void SetClearColor(float r, float g, float b, float a) = 0;

        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

        virtual void DrawHitpoint(const glm::vec3& center, float radius, const glm::vec4& color, const glm::mat4& viewMatrix, const glm::mat4& projMatrix) = 0;
        virtual void DrawLine(const glm::vec3& startingPoint, const glm::vec3& endPoint, const glm::vec4& color, const glm::mat4& viewMatrix, const glm::mat4& projMatrix) = 0;
    };
}