#include "Engine/Rendering/RendererAPI.h"

#include <Engine/Platform/OpenGL/OpenGLRendererAPI.h>

#include <Engine/Core/Log/Logger.h>

namespace Engine::Rendering
{
	RendererAPI::API RendererAPI::GetAPI()
	{
		return API::OpenGL;
	}

	RendererAPI* RendererAPI::Create()
	{
		//CRTN_LOG_DEBUG("[RENDERER API]: Initializing Renderer API");

		switch (GetAPI())
		{
			case API::OpenGL:
			{
				return new Engine::Platform::OpenGL::OpenGLRendererAPI();
				CRTN_LOG_DEBUG("[RENDERER API]: OpenGL context started");
			}

			case API::None:
			default:
				CRTN_ASSERT(false, "Unknown API!");
				return nullptr;
		}
	}
}
