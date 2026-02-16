#pragma once

#include <Engine/Core/Framebuffer.h>

namespace Engine::Platform::OpenGL
{
	class OpenGLFramebuffer : public Engine::Core::Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpec& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

		virtual uint32_t GetWidth() const override { return m_Spec.Width; }
		virtual uint32_t GetHeight() const override { return m_Spec.Height; }
	private:
		uint32_t m_RendererID = 0;
	
		FramebufferSpec m_Spec;

		uint32_t m_ColorAttachment = 0;
		uint32_t m_DepthAttachment = 0;
	};
}