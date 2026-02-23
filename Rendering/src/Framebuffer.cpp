#include "Engine/Rendering/Framebuffer.h"

#include <Engine/Core/RendererAPI.h>
#include <Engine/Core/RendererFactory.h>

#include <Engine/Core/Log/Logger.h>

#include <Engine/Platform/OpenGL/OpenGLFramebuffer.h>
#include <Engine/Platform/OpenGL/OpenGLRendererAPI.h>

namespace Engine::Rendering
{
	std::unique_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpec& spec)
	{
		if (Engine::Core::RendererFactory::Is<Engine::Platform::OpenGL::OpenGLRendererAPI>())
		{
			auto fbo = std::make_unique<Engine::Platform::OpenGL::OpenGLFramebuffer>(spec);
			if (!fbo)
			{
				CRTN_LOG_CRITICAL("<Framebuffer::Create>: Failed to create OpenGL Framebuffer!");
				CRTN_ASSERT(!fbo, "<Framebuffer::Create>: Framebuffer not initialized!");
		
				return nullptr;
			}

			return fbo;
		}
	}
}