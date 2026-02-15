#pragma once

#include <Engine/Core/Log/Logger.h>

#include <Engine/Rendering/Array/Array.h>
#include <Engine/Rendering/Buffer/Buffer.h>

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

		const Engine::Framework::Geometry::Mesh& GetMeshCPU() const { return m_MeshCPU; }
	private:
		Engine::Framework::Geometry::Mesh m_MeshCPU;

		std::shared_ptr<Array::VertexArray> m_MeshVAO;

		std::shared_ptr<Buffer::VertexBuffer> m_MeshVBO;
		std::shared_ptr<Buffer::IndexBuffer> m_MeshIBO;
	};
}