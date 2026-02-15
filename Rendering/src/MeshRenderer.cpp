#include "Engine/Rendering/MeshRenderer.h"

#include "Engine/Rendering/Renderer.h"
#include "Engine/Rendering/MeshGPU.h"

#include <Engine/Framework/Transform.h>
#include <Engine/Framework/Scene.h>
#include <Engine/Framework/Entity.h>

#include <Engine/Core/Log/Logger.h>

namespace Engine::Rendering
{
	void MeshRenderer::InitUniforms()
	{
		if (m_UniformsInitialized)
			return;

		auto& shader = m_MeshMat->GetShaderID();
		shader->Bind();

		m_MeshUniforms.Model = shader->GetUniformLocation(shader->GetDefaultUniformNames(UniformType::Model));
		m_MeshUniforms.Normal = shader->GetUniformLocation(shader->GetDefaultUniformNames(UniformType::Normal));

		m_UniformsInitialized = true;
	}

	void MeshRenderer::Draw(Engine::Framework::Transform& transform) const
	{
        if (!m_Mesh || !m_MeshMat)
            return;

        if (!m_UniformsInitialized)
            const_cast<MeshRenderer*>(this)->InitUniforms();

        m_MeshMat->Bind();

        auto& shader = m_MeshMat->GetShaderID();

        Renderer::UploadSceneUniforms(shader);

        if (transform.WasModifiedThisFrame())
        {
            const glm::mat4& model = transform.GetWorldMatrix();
            const glm::mat3 normal = glm::transpose(glm::inverse(glm::mat3(model)));

            shader->DefineUniformMat4(m_MeshUniforms.Model, model);
            shader->DefineUniformMat3(m_MeshUniforms.Normal, normal);
        }

        m_Mesh->Bind();
        m_Mesh->Draw();
	}
}