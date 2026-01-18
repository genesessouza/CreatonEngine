#include "Engine/Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

#include <Engine/Core/Log/Logger.h>

namespace Engine::Platform::OpenGL
{
	OpenGLVertexBuffer::OpenGLVertexBuffer() : m_RendererId(0), m_Data(nullptr), m_SizeInBytes(0)
	{
		CRTN_LOG_DEBUG("[OPEN GL VERTEX BUFFER]: Generating Vertex Buffer");

		glGenBuffers(1, &m_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, size_t sizeInBytes)
		: m_RendererId(0), m_Data(data), m_SizeInBytes(sizeInBytes)
	{
		CRTN_LOG_DEBUG("[OPEN GL VERTEX BUFFER]: Generating Vertex Buffer");

		glGenBuffers(1, &m_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)sizeInBytes, data, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererId);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer() : m_Count(0)
	{
		CRTN_LOG_DEBUG("[OPEN GL INDEX BUFFER]: Generating Index Buffer");

		glGenBuffers(1, &m_RendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(const void* data, size_t size)
		: m_Count(size)
	{
		CRTN_LOG_DEBUG("[OPEN GL INDEX BUFFER]: Generating Index Buffer");

		glGenBuffers(1, &m_RendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererId);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}