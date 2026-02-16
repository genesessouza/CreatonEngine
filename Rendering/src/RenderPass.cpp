#include "Engine/Rendering/RenderPass.h"

#include "Engine/Rendering/RendererAPI.h"

#include <Engine/Platform/OpenGL/OpenGLRenderPass.h>

namespace Engine::Rendering
{
	std::unique_ptr<RenderPass> RenderPass::Create()
	{
		switch (Engine::Rendering::RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_unique<Engine::Platform::OpenGL::OpenGLRenderPass>();
		}
	}

	std::unique_ptr<ShadowPass> ShadowPass::Create()
	{
		switch (Engine::Rendering::RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_unique<Engine::Platform::OpenGL::OpenGLShadowPass>();
		}
	}

	std::unique_ptr<PostProcessPass> PostProcessPass::Create()
	{
		switch (Engine::Rendering::RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;

		case RendererAPI::API::OpenGL:
			return std::make_unique<Engine::Platform::OpenGL::OpenGLPostProcessPass>();
		}
	}
}