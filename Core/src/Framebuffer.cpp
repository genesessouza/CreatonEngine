#include "Engine/Core/Framebuffer.h"
#include "Engine/Core/Log/Logger.h"

#include "Engine/Platform/OpenGL/OpenGLFramebuffer.h"

#include <Engine/Rendering/RendererAPI.h>

namespace Engine::Core
{
	std::unique_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpec& spec)
	{
		switch (Engine::Rendering::RendererAPI::GetAPI())
		{
			case Engine::Rendering::RendererAPI::API::None:    return nullptr;
			case Engine::Rendering::RendererAPI::API::OpenGL:  return std::make_unique<Engine::Platform::OpenGL::OpenGLFramebuffer>(spec);
		}

		CRTN_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}