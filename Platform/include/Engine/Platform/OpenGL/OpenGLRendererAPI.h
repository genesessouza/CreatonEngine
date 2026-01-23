#pragma once

#include <Engine/Rendering/RendererAPI.h>

#include <cstdint>

namespace Engine::Platform::OpenGL
{
	class OpenGLRendererAPI : public Engine::Rendering::RendererAPI
	{
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		void Clear();
	};
}