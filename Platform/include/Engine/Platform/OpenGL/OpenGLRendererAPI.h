#pragma once

#include <Engine/Core/RendererAPI.h>

namespace Engine::Platform::OpenGL
{
	class OpenGLRendererAPI : public Engine::Core::RendererAPI
	{
		void Clear() override;
		void SetClearColor(float r, float g, float b, float a) override;

		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		void DrawHitpoint(const glm::vec3& center, float radius, const glm::vec4& color, const glm::mat4& viewMatrix, const glm::mat4& projMatrix) override;
		void DrawLine(const glm::vec3& startingPoint, const glm::vec3& endPoint, const glm::vec4& color, const glm::mat4& viewMatrix, const glm::mat4& projMatrix) override;
	};
}