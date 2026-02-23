#pragma once

#include "Engine/Core/RendererAPI.h"

#include <cstdint>

namespace Engine::Core
{
	class RenderCommand
	{
	public:
		static void Init(RendererAPI* api);

		static void Clear();
		static void SetClearColor(float r, float g, float b, float a);

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static RendererAPI* GetRendererAPI() { return s_API; }
	private:
		static RendererAPI* s_API;
	};
}