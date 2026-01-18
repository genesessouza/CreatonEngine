#include "Engine/Rendering/MeshGPU.h"

#include <Engine/Framework/Mesh.h>

#include <Engine/Core/Log/Logger.h>

namespace Engine::Rendering
{
	MeshGPU::MeshGPU(const Engine::Framework::Geometry::Mesh& mesh)
	{
		CRTN_LOG_DEBUG("[MESH GPU]: Constructing geometry data...\n");

		m_MeshVAO = Engine::Rendering::Array::VertexArray::Create();
		m_MeshVBO = Engine::Rendering::Buffer::VertexBuffer::Create(mesh.vertices.data(), mesh.vertices.size() * sizeof(Engine::Framework::Geometry::Vertex));
		m_MeshIBO = Engine::Rendering::Buffer::IndexBuffer::Create(mesh.indices.data(), mesh.indices.size() * sizeof(uint32_t));

		m_MeshVAO->AddVertexBuffer(m_MeshVBO, Engine::Framework::Geometry::Vertex::GetLayout());
		m_MeshVAO->SetIndexBuffer(m_MeshIBO);

		m_MeshVAO->Unbind();

		CRTN_LOG_DEBUG("[MESH GPU]: Done\n");
	}

	void MeshGPU::Bind() const
	{
		m_MeshVAO->Bind();
	}
}