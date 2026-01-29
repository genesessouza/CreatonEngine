#pragma once

#include <Engine/Platform/OpenGL/OpenGLArray.h>
#include <Engine/Platform/OpenGL/OpenGLBuffer.h>

#include <Engine/Framework/Mesh.h>

namespace Engine::Rendering
{
	class MeshGPU
	{
	public:
		MeshGPU(const Engine::Framework::Geometry::Mesh& mesh);

		void Bind() const;

		void Draw() const;

		size_t GetIndexCount() 
		{ 
			CRTN_CHECK_PTR(m_MeshIBO);
			return m_MeshIBO->GetCount(); 
		}
	private:
		std::shared_ptr<Engine::Rendering::Array::VertexArray> m_MeshVAO;

		std::shared_ptr<Engine::Rendering::Buffer::VertexBuffer> m_MeshVBO;
		std::shared_ptr<Engine::Rendering::Buffer::IndexBuffer> m_MeshIBO;
	};
}