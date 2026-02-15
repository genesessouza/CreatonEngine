#include "Engine/Framework/Billboard.h"

#include "Engine/Framework/Camera.h"
#include "Engine/Framework/Mesh.h"
#include "Engine/Framework/Entity.h"

#include <Engine/Rendering/Array/Array.h>
#include <Engine/Rendering/Buffer/Buffer.h>
#include <Engine/Rendering/Shader.h>
#include <Engine/Rendering/Renderer.h>

#include <Engine/Core/Log/Logger.h>

namespace Engine::Framework
{
	void Billboard::Init()
	{
		InitUniforms();

		float quad[] = {
			-0.5f, -0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f
		};

		uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };

		m_VAO = Engine::Rendering::Array::VertexArray::Create();

		m_VBO = Engine::Rendering::Buffer::VertexBuffer::Create(quad, sizeof(quad));
		m_IBO = Engine::Rendering::Buffer::IndexBuffer::Create(indices, 6);

		m_VAO->AddVertexBuffer(m_VBO, Geometry::Vertex::GetBillboardLayout());
		m_VAO->SetIndexBuffer(m_IBO);

		m_VAO->Unbind();
	}

	void Billboard::InitUniforms()
	{
		if (m_UniformsInitialized)
			return;

		m_Shader = Engine::Rendering::Shader::CreateBillboardShader();
		m_Shader->Bind();

		m_BillboardUniforms.View = m_Shader->GetUniformLocation(m_Shader->GetDefaultUniformNames(Engine::Rendering::UniformType::View));
		m_BillboardUniforms.ViewProjection = m_Shader->GetUniformLocation(m_Shader->GetDefaultUniformNames(Engine::Rendering::UniformType::ViewProjection));

		m_BillboardUniforms.Position = m_Shader->GetUniformLocation("u_Position");
		m_BillboardUniforms.Size = m_Shader->GetUniformLocation("u_Size");
		m_BillboardUniforms.Color = m_Shader->GetUniformLocation("u_Color");

		m_UniformsInitialized = true;
	}

	void Billboard::Draw(const Transform& transform, const Camera& sceneCamera)
	{
		if (!m_UniformsInitialized)
			const_cast<Billboard*>(this)->InitUniforms();

		m_Shader->Bind();

		auto& sceneData = Engine::Rendering::Renderer::GetSceneData();

		const glm::mat4& view = sceneCamera.GetViewMatrix();
		
		if (sceneData.CameraDirty)
		{
			m_Shader->DefineUniformMat4(m_BillboardUniforms.View, view);
			m_Shader->DefineUniformMat4(m_BillboardUniforms.ViewProjection, sceneData.ViewProjection);
		}

		if (sceneData.LightsDirty)
		{
			m_Shader->DefineUniformVec3(m_BillboardUniforms.Position, transform.GetPosition());
			m_Shader->DefineUniformFloat(m_BillboardUniforms.Size, m_Size);
			m_Shader->DefineUniformVec4(m_BillboardUniforms.Color, m_Color);
		}

		m_VAO->Bind();

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_IBO->GetCount()), GL_UNSIGNED_INT, nullptr);
	}
}