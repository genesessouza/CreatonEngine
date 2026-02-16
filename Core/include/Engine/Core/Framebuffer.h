#pragma once

#include <cstdint>
#include <memory>

namespace Engine::Core
{
	class Framebuffer
	{
	public:
		struct FramebufferSpec
		{
			uint32_t Width, Height;

			bool HDR = false;
			bool HasColor = true;
			bool HasDepth = true;
			bool DepthOnly = false;
		};
	public:
		Framebuffer() = default;
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		static std::unique_ptr<Framebuffer> Create(const FramebufferSpec& spec);
	};
}