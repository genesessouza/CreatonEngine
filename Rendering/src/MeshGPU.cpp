#include "Engine/Rendering/MeshGPU.h"

#include "Engine/Rendering/Buffer/VertexBufferLayout.h"

#include <Engine/Core/Log/Logger.h>

namespace Engine::Rendering
{
	MeshGPU::MeshGPU(const Engine::Framework::Geometry::Mesh& mesh)
	{
		CRTN_LOG_DEBUG("[MESH GPU]: Constructing geometry data...\n");

		m_MeshVAO = Array::VertexArray::Create();
		m_MeshVBO = Buffer::VertexBuffer::Create(mesh.GetVertices().data(), mesh.GetVertices().size() * sizeof(Engine::Framework::Geometry::Vertex));
		m_MeshIBO = Buffer::IndexBuffer::Create(mesh.GetIndices().data(), mesh.GetIndices().size() * sizeof(uint32_t));

		m_MeshVAO->AddVertexBuffer(m_MeshVBO, GetLayout());
		m_MeshVAO->SetIndexBuffer(m_MeshIBO);

		m_MeshVAO->Unbind();

		m_MeshCPU = mesh;
		CRTN_LOG_DEBUG("[MESH GPU]: Done\n");
	}

	void MeshGPU::Bind() const
	{
		m_MeshVAO->Bind();
	}

	void MeshGPU::Draw() const
	{
		CRTN_CHECK_PTR(m_MeshIBO);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_MeshIBO->GetCount()), GL_UNSIGNED_INT, nullptr);
	}
}