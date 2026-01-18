#include "Engine/Rendering/Buffer/Buffer.h"

#include <Engine/Core/Log/Logger.h>

#include <Engine/Platform/OpenGL/OpenGLBuffer.h>

#include <memory>

namespace Engine::Rendering::Buffer
{
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(const void* data, size_t size)
	{
		auto vbo = std::make_shared<Engine::Platform::OpenGL::OpenGLVertexBuffer>(data, size);
		CRTN_ASSERT(vbo, "Vertex buffer could not be created!");
		
		return vbo;
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(const void* data, size_t size)
	{
		auto ibo = std::make_shared<Engine::Platform::OpenGL::OpenGLIndexBuffer>(data, size);
		CRTN_ASSERT(ibo, "Index buffer could not be created!");
	
		return ibo;
	}
}