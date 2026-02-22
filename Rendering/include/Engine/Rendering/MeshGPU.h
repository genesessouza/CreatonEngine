#pragma once

#include <Engine/Core/Log/Logger.h>

#include "Engine/Rendering/Array/Array.h"
#include "Engine/Rendering/Buffer/Buffer.h"

#include <Engine/Framework/Mesh.h>

namespace Engine::Rendering
{
	class MeshGPU
	{
	public:
		MeshGPU(const Engine::Framework::Geometry::Mesh& mesh);

		void Bind() const;

		void Draw() const;

		Array::VertexArray* GetVAO() const { return m_MeshVAO.get(); }
		Buffer::VertexBuffer* GetVBO() const { return m_MeshVBO.get(); }
		Buffer::IndexBuffer* GetIBO() const { return m_MeshIBO.get(); }

		size_t GetIndexCount() 
		{ 
			CRTN_CHECK_PTR(m_MeshIBO);
			return m_MeshIBO->GetCount(); 
		}

		static Buffer::VertexBufferLayout GetLayout()
		{
			Buffer::VertexBufferLayout layout;

			layout.Add(0, 3, GL_FLOAT, false); // position
			layout.Add(1, 3, GL_FLOAT, false); // normal
			layout.Add(2, 2, GL_FLOAT, false); // uv

			return layout;
		}

		static Buffer::VertexBufferLayout GetBillboardLayout()
		{
			Engine::Rendering::Buffer::VertexBufferLayout layout;

			layout.Add(0, 3, GL_FLOAT, false); // offset

			return layout;
		}

		const Engine::Framework::Geometry::Mesh& GetMeshCPU() const { return m_MeshCPU; }
		Engine::Framework::Geometry::Mesh GetMeshCPU() { return m_MeshCPU; }
	private:
		Engine::Framework::Geometry::Mesh m_MeshCPU;

		std::shared_ptr<Array::VertexArray> m_MeshVAO;

		std::shared_ptr<Buffer::VertexBuffer> m_MeshVBO;
		std::shared_ptr<Buffer::IndexBuffer> m_MeshIBO;
	};
}