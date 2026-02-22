#pragma once

#include "Engine/Rendering/Shader.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <cstdint>
#include <memory>
#include <Engine/Core/Log/Logger.h>

namespace Engine::Rendering
{
	class MeshGPU;
	class RenderCommand;

	class RenderPass
	{
	public:
		RenderPass() : m_VAO(0), m_VBO(0), m_Width(0), m_Height(0) {}
		virtual ~RenderPass() = default;

		virtual void Begin() = 0;
		virtual void End() = 0;

		virtual void Init() = 0;
		virtual void Init(uint32_t width, uint32_t height) = 0;

		virtual void Render(uint32_t texture) = 0;

		void SetShader(std::shared_ptr<Shader> shader)
		{
			m_Shader = shader;
		}

		static std::unique_ptr<RenderPass> Create();
	protected:
		uint32_t m_Width, m_Height;

		uint32_t m_VAO, m_VBO;
		std::shared_ptr<Shader> m_Shader;
	};

	class ShadowPass : public RenderPass
	{
	public:
		ShadowPass() : RenderPass(), m_LightView(0), m_LightSpaceMatrix(1.0f), m_LightProjection(1.0f) {}
		virtual ~ShadowPass() = default;

		virtual void Begin() override = 0;
		virtual void End() override = 0;

		void Init() override {}
		virtual void Init(uint32_t width, uint32_t height) override = 0;

		virtual void Render(const Engine::Rendering::RenderCommand& cmd) = 0;

		const glm::mat4& GetDirectionalLightSpaceMatrix() const { return m_LightSpaceMatrix; }

		virtual void CalculateDirectionalLightSpaceMatrix(glm::vec3 lightDir, glm::vec3 cameraPos, float cameraNear, float cameraFar) = 0;

		static std::unique_ptr<ShadowPass> Create();
	private:
		void Render(uint32_t texture) override {}
	protected:
		glm::mat4 m_LightProjection;
		glm::mat4 m_LightView;
		glm::mat4 m_LightSpaceMatrix;

		std::shared_ptr<Engine::Rendering::Shader> m_DepthShader;
	};

	class PostProcessPass : public RenderPass
	{
	public:
		PostProcessPass() : RenderPass() {}
		virtual ~PostProcessPass() = default;

		virtual void Begin() override = 0;
		virtual void End() override = 0;
				
		void Init() override {}
		virtual void Init(uint32_t width, uint32_t height) override = 0;
		
		uint32_t GetColorTexture() const { return m_ColorTexture; }
		
		void SetExposure(float exposure) { m_Exposure = exposure; }

		void Render(uint32_t texture) override = 0;

		static std::unique_ptr<PostProcessPass> Create();
	protected:
		uint32_t m_FBO = 0;
		uint32_t m_ColorTexture = 0;
		uint32_t m_RBO = 0; // depth/stencil
		float m_Exposure = 1.0f;
	};
}