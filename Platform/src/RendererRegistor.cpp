#include "Engine/Platform/RendererRegistor.h"

#include <Engine/Core/RendererFactory.h>
#include <Engine/Core/Log/Logger.h>

#include <Engine/Platform/OpenGL/OpenGLRendererAPI.h>

void RegisterRendererAPI(Engine::Platform::API targetApi)
{
	switch (targetApi)
	{
		case Engine::Platform::API::None:
			CRTN_LOG_CRITICAL("No Valid RendererAPI Selected!");
			return;
			break;

		case Engine::Platform::API::OpenGl:
			Engine::Core::RendererFactory::RegisterRenderer([]()
				{
					CRTN_LOG_TRACE("<RendererRegistor::RegisterRendererAPI>: OpenGL RendererAPI Initialized");
					return std::make_unique<Engine::Platform::OpenGL::OpenGLRendererAPI>();
				});
			break;

		case Engine::Platform::API::Vulkan:
			// Maybe one day...
			break;
	}
}