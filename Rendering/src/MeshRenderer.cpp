#include "Engine/Rendering/MeshRenderer.h"

#include <Engine/Core/Log/Logger.h>

namespace Engine::Rendering
{
	void MeshRenderer::Draw(const Engine::Framework::Transform& transform, const Engine::Framework::Camera& camera) const
	{
		//CRTN_LOG_INFO("MeshRenderer::Render() called");

		m_Shader->Bind();

		//m_Shader->DefineUniformMat4("u_ModelMatrix", glm::mat4(1.0f));
		m_Shader->DefineUniformMat4(m_Shader->GetDefaultUniformNames(UniformType::Model), transform.GetMatrix());

		//m_Shader->DefineUniformMat4(m_Shader->GetDefaultUniformNames(UniformType::View), camera.GetViewMatrix());
		m_Shader->DefineUniformMat4(m_Shader->GetDefaultUniformNames(UniformType::ViewProjection), camera.GetViewProjectionMatrix());

		m_Mesh->Bind();

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, m_Mesh->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	}
}