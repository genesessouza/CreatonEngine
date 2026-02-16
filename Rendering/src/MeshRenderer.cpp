#include "Engine/Rendering/MeshRenderer.h"

#include "Engine/Rendering/Renderer.h"

#include <Engine/Framework/Transform.h>
#include <Engine/Framework/Scene.h>
#include <Engine/Framework/Entity.h>
#include <Engine/Framework/Mesh.h>

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

		if (transform.WasModifiedThisFrame())
			Renderer::Submit(m_Mesh.get(), m_MeshMat.get(), transform.GetWorldMatrix());
	}
}