#include "Engine/Rendering/Array/Array.h"

#include <Engine/Platform/OpenGL/OpenGLArray.h>
#include <Engine/Core/Log/Logger.h>

namespace Engine::Rendering::Array
{
	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		auto vao = std::make_shared<Engine::Platform::OpenGL::OpenGLVertexArray>();
		CRTN_CHECK_PTR(vao);
		CRTN_ASSERT(vao, "Vertex Array could not be created!");
	
		return vao;
	}
}