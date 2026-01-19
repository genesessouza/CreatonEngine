#include "Engine/Rendering/MeshRenderer.h"
#include <Engine/Rendering/Renderer.h>

#include <Engine/Core/Log/Logger.h>

namespace Engine::Rendering
{
	void MeshRenderer::Draw(const Engine::Framework::Transform& transform) const
	{
        Engine::Rendering::Renderer::SceneData sceneData = Engine::Rendering::Renderer::GetSceneData();

        m_MeshMat->Bind();
        auto shader = m_MeshMat->GetShader();

        //shader->DefineUniformMat3("u_NormalMatrix", glm::transpose(glm::inverse(glm::mat3(transform.GetMatrix()))));
        shader->DefineUniformMat4("u_ModelMatrix", transform.GetMatrix());
        shader->DefineUniformMat4("u_ViewProjectionMatrix", sceneData.ViewProjection);
        shader->DefineUniformVec3("u_ViewPos", sceneData.ViewPos);

        shader->DefineUniformBool("u_HasDirLight", sceneData.HasDirLight);
        
        if (sceneData.HasDirLight)
        {
            shader->DefineUniformVec3("u_DirLightDir", sceneData.DirLightDirection);
            shader->DefineUniformVec4("u_DirLightColor", sceneData.DirLightColor);
        }

        int count = (int)sceneData.PointLights.size();
        shader->DefineUniformInt("u_PointLightCount", count);

        for (int i = 0; i < count; i++) 
        {
            std::string prefix = "u_PointLights[" + std::to_string(i) + "].";
        
            shader->DefineUniformVec3(prefix + "Position", sceneData.PointLights[i].Position);
            shader->DefineUniformVec4(prefix + "Color", sceneData.PointLights[i].Color);
            shader->DefineUniformFloat(prefix + "Intensity", sceneData.PointLights[i].Intensity);
        }

		m_Mesh->Bind();

		glDrawElements(GL_TRIANGLES, m_Mesh->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	}
}