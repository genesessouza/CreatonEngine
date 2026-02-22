#pragma once

#include <Engine/Rendering/RenderPass.h>

namespace Engine::Rendering
{
	class RenderCommand;
	class MeshGPU;	
}
namespace Engine::Platform::OpenGL
{
	class OpenGLRenderPass : public Engine::Rendering::RenderPass
	{
	public:
		OpenGLRenderPass() : RenderPass() {}
		virtual ~OpenGLRenderPass() = default;

		void Begin() override;
		void End() override;

		void Init() override {}
		void Init(uint32_t width, uint32_t height) override;
	private:
		void Render(uint32_t texture) override {}
	};

	class OpenGLShadowPass : public Engine::Rendering::ShadowPass
	{
	public:
		OpenGLShadowPass() : ShadowPass() {}
		virtual ~OpenGLShadowPass() = default;

		void Begin() override;
		void End() override;

		void Init() override {}
		void Init(uint32_t width, uint32_t height) override;

		void CalculateDirectionalLightSpaceMatrix(glm::vec3 lightDir, glm::vec3 cameraPos, float cameraNear, float cameraFar) override;

		void Render(const Engine::Rendering::RenderCommand& cmd) override;
	private:
		void Render(uint32_t texture) override {}
	};

	class OpenGLPostProcessPass : public Engine::Rendering::PostProcessPass
	{
	public:
		OpenGLPostProcessPass() : PostProcessPass() {}
		virtual ~OpenGLPostProcessPass() = default;

		void Begin() override;
		void End() override;

		void Init() override {}
		void Init(uint32_t width, uint32_t height) override;

		void Render(uint32_t texture) override;
	};
}