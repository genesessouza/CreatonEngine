#pragma once

#include <cstdint>
#include <memory>

namespace Engine::Rendering
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL
		};

		virtual ~RendererAPI() = default;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void Clear() = 0;

		static API GetAPI();
		static std::unique_ptr<RendererAPI> Create();
	};
}