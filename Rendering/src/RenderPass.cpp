#include "Engine/Rendering/RenderPass.h"

#include "Engine/Core/RendererAPI.h"
#include "Engine/Core/RendererFactory.h"

#include <Engine/Platform/OpenGL/OpenGLRenderPass.h>
#include <Engine/Platform/OpenGL/OpenGLRendererAPI.h>

namespace Engine::Rendering
{
	std::unique_ptr<RenderPass> RenderPass::Create()
	{
		if (Engine::Core::RendererFactory::Is<Engine::Platform::OpenGL::OpenGLRendererAPI>())
		{
			auto renderPass = std::make_unique<Engine::Platform::OpenGL::OpenGLRenderPass>();
			CRTN_ASSERT(renderPass, "[RENDER PASS]: Failed to create <OpenGL> Render Pass!");
			
			return renderPass;
		}
	}

	std::unique_ptr<ShadowPass> ShadowPass::Create()
	{
		if (Engine::Core::RendererFactory::Is<Engine::Platform::OpenGL::OpenGLRendererAPI>())
		{
			auto shadowPass = std::make_unique<Engine::Platform::OpenGL::OpenGLShadowPass>();
			CRTN_ASSERT(shadowPass, "[SHADOW PASS]: Failed to create <OpenGL> Shadow Pass!");

			return shadowPass;
		}
	}

	std::unique_ptr<PostProcessPass> PostProcessPass::Create()
	{
		if (Engine::Core::RendererFactory::Is<Engine::Platform::OpenGL::OpenGLRendererAPI>())
		{
			auto postProcessPass = std::make_unique<Engine::Platform::OpenGL::OpenGLPostProcessPass>();
			CRTN_ASSERT(postProcessPass, "[POST PROCESS PASS]: Failed to create <OpenGL> PostProcess Pass!");

			return postProcessPass;
		}
	}
}