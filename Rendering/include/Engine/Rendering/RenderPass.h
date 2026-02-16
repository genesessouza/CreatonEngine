#pragma once

#include "Engine/Rendering/Shader.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <cstdint>
#include <memory>

namespace Engine::Rendering
{
	class MeshGPU;
	class RenderCommand;

	class RenderPass
	{
	public:
		RenderPass() : m_VAO(0), m_VBO(0) {}
		virtual ~RenderPass() = default;

		virtual void Init() = 0;
		virtual void Begin() = 0;
		virtual void End() = 0;

		virtual void Render(uint32_t texture) = 0;

		void SetShader(std::shared_ptr<Shader> shader)
		{
			m_Shader = shader;
		}

		static std::unique_ptr<RenderPass> Create();
	protected:
		uint32_t m_VAO, m_VBO;
		std::shared_ptr<Shader> m_Shader;
	};

	class ShadowPass : public RenderPass
	{
	public:
		ShadowPass() : RenderPass(), m_Height(0), m_Width(0), m_LightView(0), m_LightSpaceMatrix(1.0f), m_LightProjection(1.0f) {}
		virtual ~ShadowPass() = default;

		void Init() override {}

		virtual void Init(uint32_t width, uint32_t height) = 0;
		virtual void Begin() override = 0;
		virtual void End() override = 0;

		virtual void Render(const Engine::Rendering::RenderCommand& cmd) = 0;

		uint32_t GetDepthTexture() const { return m_DepthTexture; }
		const glm::mat4& GetLightSpaceMatrix() const { return m_LightSpaceMatrix; }

		// FOR DIRECTIONAL LIGHT
		void UpdateLightMatrix(glm::vec3 lightDir)
		{
			float near_plane = 1.0f;
			float far_plane = 100.0f;

			m_LightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);

			glm::vec3 lightPos = -lightDir * 30.0f;

			m_LightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));

			m_LightSpaceMatrix = m_LightProjection * m_LightView;
		}

		static std::unique_ptr<ShadowPass> Create();
	private:
		void Render(uint32_t texture) override {}
	protected:
		uint32_t m_Width, m_Height;

		uint32_t m_FBO = 0;
		uint32_t m_DepthTexture = 0;

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
				
		void Init() override {}

		virtual void Init(uint32_t width, uint32_t height) = 0;
		virtual void Begin() override = 0;
		virtual void End() override = 0;
		
		void Render(uint32_t texture) override = 0;

		static std::unique_ptr<PostProcessPass> Create();
	protected:
		uint32_t m_Width = 0, m_Height = 0;

		uint32_t m_FBO = 0;
		uint32_t m_ColorTexture = 0;
		uint32_t m_RBO = 0; // depth/stencil
	};
}