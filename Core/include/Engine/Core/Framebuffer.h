#pragma once

#include <cstdint>
#include <memory>

#include <glm/glm.hpp>

namespace Engine::Core
{
	class Framebuffer
	{
	public:
		struct FramebufferSpec
		{
			uint32_t Width = 512, Height = 512;

			bool HDR = false;
			bool HasColor = false;
			bool HasDepth = false;
			bool DepthOnly = false;
		};
	public:
		Framebuffer() = default;
		virtual ~Framebuffer() = default;

		virtual void Invalidate() = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual FramebufferSpec GetFramebufferSpec() const = 0;
		virtual const void SetFramebufferSpec(const FramebufferSpec& spec) = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;
		virtual uint32_t GetDepthAttachmentRendererID() const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		static std::unique_ptr<Framebuffer> Create(const FramebufferSpec& spec);
	};
}