#include "Engine/Rendering/MeshRenderer.h"
#include "Engine/Rendering/Renderer.h"

#include <Engine/Core/Log/Logger.h>

namespace Engine::Rendering
{
	void MeshRenderer::InitUniforms()
	{
		auto& shader = m_MeshMat->GetShaderID();
		shader->Bind();

		m_MeshUniforms.Model = shader->GetUniformLocation(shader->GetDefaultUniformNames(UniformType::Model));
		m_MeshUniforms.Normal = shader->GetUniformLocation(shader->GetDefaultUniformNames(UniformType::Normal));

		m_UniformsInitialized = true;
	}

	void MeshRenderer::Draw(const Engine::Framework::Transform& transform) const
	{
		const auto& sceneData = Engine::Rendering::Renderer::GetSceneData();

		// PREVENTS INVALID/GARBAGE MESH MATRICES IN CASE INIT_UNIFORMS() WASN'T CALLED ANYWHERE
		if (m_UniformsInitialized == false)
			const_cast<MeshRenderer*>(this)->InitUniforms();

		m_MeshMat->Bind();

		auto& shader = m_MeshMat->GetShaderID();

		Engine::Rendering::Renderer::UploadSceneUniforms(shader);

		if (transform.IsDirty())
		{
			const glm::mat4& transformModel = transform.GetMatrix();
			glm::mat3 transformNormal = glm::transpose(glm::inverse(glm::mat3(transformModel)));

			shader->DefineUniformMat4(m_MeshUniforms.Model, transformModel);
			shader->DefineUniformMat3(glGetUniformLocation(shader->GetShader(), shader->GetDefaultUniformNames(UniformType::Normal).c_str()), transformNormal);

			transform.ClearDirty();
		}

		m_Mesh->Bind();

		glDrawElements(GL_TRIANGLES, m_Mesh->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	}
}