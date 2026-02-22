#pragma once

#include <Engine/Core/Framebuffer.h>

namespace Engine::Platform::OpenGL
{
	class OpenGLFramebuffer : public Engine::Core::Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpec& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate() override;

		void Bind() override;
		void Unbind() override;

		void Resize(uint32_t width, uint32_t height) override;

		FramebufferSpec GetFramebufferSpec() const override { return m_Spec; }
		const void SetFramebufferSpec(const FramebufferSpec& spec) override { m_Spec = spec; }

		uint32_t GetColorAttachmentRendererID() const override { if(!m_Spec.DepthOnly) return m_ColorAttachment; } // Fail safe in case of shadow fbo
		uint32_t GetDepthAttachmentRendererID() const override { return m_DepthAttachment; }

		uint32_t GetWidth() const override { return m_Spec.Width; }
		uint32_t GetHeight() const override { return m_Spec.Height; }
	private:
		uint32_t m_RendererID = 0;
	
		FramebufferSpec m_Spec;

		uint32_t m_ColorAttachment = 0;
		uint32_t m_DepthAttachment = 0;
	};
}