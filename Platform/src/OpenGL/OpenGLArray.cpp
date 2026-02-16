#include "Engine/Platform/OpenGL/OpenGLArray.h"

#include <Engine/Rendering/Buffer/VertexBufferLayout.h>

#include <Engine/Core/Log/Logger.h>

#include <glad/glad.h>

#include <iostream>

namespace Engine::Platform::OpenGL
{
	OpenGLVertexArray::OpenGLVertexArray()
	{
		CRTN_LOG_DEBUG("[OPEN GL VERTEX ARRAY]: Generating VAO");

		glGenVertexArrays(1, &m_RendererId);
		Bind();
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererId);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererId);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<Engine::Rendering::Buffer::VertexBuffer>& vertexBuffer, const Engine::Rendering::Buffer::VertexBufferLayout& layout)
	{
		CRTN_CHECK_PTR(vertexBuffer);

		CRTN_LOG_DEBUG("[OPEN GL VERTEX ARRAY]: Adding buffers to VAO");

		glBindVertexArray(m_RendererId);
		vertexBuffer->Bind();

		const auto& elements = layout.GetElements();

		for (const auto& element : elements) {
			glEnableVertexAttribArray(element.index);
			glVertexAttribPointer(
				element.index,
				element.count,
				element.type,
				element.normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)(uintptr_t)element.offset
			);
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<Engine::Rendering::Buffer::IndexBuffer>& indexBuffer)
	{
		CRTN_CHECK_PTR(indexBuffer);

		CRTN_LOG_DEBUG("[OPEN GL VERTEX ARRAY]: Setting Index Buffer on VAO\n");

		glBindVertexArray(m_RendererId);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}