#pragma once

#include <Engine/Core/Log/Logger.h>

#include <Engine/Rendering/Array/Array.h>
#include <Engine/Rendering/Buffer/Buffer.h>

namespace Engine::Rendering::Buffer
{
	class VertexBufferLayout;
}

namespace Engine::Platform::OpenGL
{
	class OpenGLVertexArray : public Engine::Rendering::Array::VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<Engine::Rendering::Buffer::VertexBuffer>& vertexBuffer, const Engine::Rendering::Buffer::VertexBufferLayout& layout) override;
		virtual void SetIndexBuffer(const std::shared_ptr<Engine::Rendering::Buffer::IndexBuffer>& indexBuffer) override;

		virtual const std::vector<std::shared_ptr<Engine::Rendering::Buffer::VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const std::shared_ptr<Engine::Rendering::Buffer::IndexBuffer>& GetIndexBuffer() const override
		{
			CRTN_CHECK_PTR(m_IndexBuffer);
			return m_IndexBuffer;
		}
	private:
		uint32_t m_RendererId;

		std::vector<std::shared_ptr<Engine::Rendering::Buffer::VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<Engine::Rendering::Buffer::IndexBuffer> m_IndexBuffer;
	};
}