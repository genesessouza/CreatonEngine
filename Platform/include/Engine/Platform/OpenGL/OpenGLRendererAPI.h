#pragma once

#include <Engine/Rendering/RendererAPI.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <cstdint>

namespace Engine::Platform::OpenGL
{
	class OpenGLRendererAPI : public Engine::Rendering::RendererAPI
	{
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		void Clear();

		void DrawHitpoint(const glm::vec3& center, float radius, const glm::vec4& color, const glm::mat4& viewMatrix, const glm::mat4& projMatrix) override;
		void DrawLine(const glm::vec3& startingPoint, const glm::vec3& endPoint, const glm::vec4& color, const glm::mat4& viewMatrix, const glm::mat4& projMatrix) override;
	};
}