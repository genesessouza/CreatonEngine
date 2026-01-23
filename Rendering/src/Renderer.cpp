#include "Engine/Rendering/Renderer.h"

#include <Engine/Framework/Camera.h>
#include <Engine/Framework/Scene.h>

namespace Engine::Rendering
{
	Renderer::SceneData Renderer::s_SceneData;

	void Renderer::InitSceneUniforms(const std::shared_ptr<Engine::Rendering::Shader>& shader)
	{
		shader->Bind();

		shader->sceneUniforms.ViewProjection = shader->GetUniformLocation(shader->GetDefaultUniformNames(UniformType::ViewProjection));
		shader->sceneUniforms.ViewPos = shader->GetUniformLocation("u_ViewPos");

		shader->sceneUniforms.HasDirLight = shader->GetUniformLocation("u_HasDirLight");
		shader->sceneUniforms.DirLightDir = shader->GetUniformLocation("u_DirLightDir");
		shader->sceneUniforms.DirLightColor = shader->GetUniformLocation("u_DirLightColor");

		shader->sceneUniforms.PointLightCount = shader->GetUniformLocation("u_PointLightCount");

		for (int i = 0; i < 8; i++)
		{
			shader->sceneUniforms.PointLightPos[i] = shader->GetUniformLocation("u_PointLights[" + std::to_string(i) + "].Position");
			shader->sceneUniforms.PointLightColor[i] = shader->GetUniformLocation("u_PointLights[" + std::to_string(i) + "].Color");
			shader->sceneUniforms.PointLightIntensity[i] = shader->GetUniformLocation("u_PointLights[" + std::to_string(i) + "].Intensity");
		}
	}

	void Renderer::UploadSceneUniforms(const std::shared_ptr<Engine::Rendering::Shader>& shader)
	{
		const auto& scene = s_SceneData;

		if (s_SceneData.CameraDirty)
		{
			shader->DefineUniformMat4(shader->sceneUniforms.ViewProjection, scene.ViewProjection);
			shader->DefineUniformVec3(shader->sceneUniforms.ViewPos, scene.ViewPos);
		}

		if (s_SceneData.LightsDirty)
		{
			shader->DefineUniformBool(shader->sceneUniforms.HasDirLight, scene.HasDirLight);

			if (scene.HasDirLight)
			{
				shader->DefineUniformVec3(shader->sceneUniforms.DirLightDir, scene.DirLightDirection);
				shader->DefineUniformVec4(shader->sceneUniforms.DirLightColor, scene.DirLightColor);
			}

			shader->DefineUniformInt(shader->sceneUniforms.PointLightCount, scene.PointLights.size());

			for (int i = 0; i < scene.PointLights.size(); i++)
			{
				shader->DefineUniformVec3(shader->sceneUniforms.PointLightPos[i], scene.PointLights[i].Position);
				shader->DefineUniformVec4(shader->sceneUniforms.PointLightColor[i], scene.PointLights[i].Color);
				shader->DefineUniformFloat(shader->sceneUniforms.PointLightIntensity[i], scene.PointLights[i].Intensity);
			}
		}
	}

	void Renderer::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::BeginScene(std::shared_ptr<Framework::Camera>& camera, const Engine::Framework::Scene& scene)
	{
		CRTN_CHECK_PTR(camera);

		s_SceneData.ViewProjection = camera->GetViewProjectionMatrix();
		s_SceneData.ViewPos = camera->GetPosition();
		s_SceneData.CameraDirty = true;

		s_SceneData.HasDirLight = false;

		if (auto& dirLight = scene.GetDirectionalLight())
		{
			s_SceneData.DirLightDirection = dirLight->GetDirection();
			s_SceneData.DirLightColor = dirLight->GetColor();
			s_SceneData.HasDirLight = true;
		}

		s_SceneData.PointLights.clear();

		const auto& pointLights = scene.GetPointLights();
		const size_t count = std::min(pointLights.size(), size_t(8));

		for (size_t i = 0; i < count; ++i)
		{
			auto* light = pointLights[i];
			s_SceneData.PointLights.push_back({ light->m_Transform.GetPosition(), light->GetColor(), light->GetIntensity() });
		}

		s_SceneData.LightsDirty = true;
	}

	void Renderer::EndScene()
	{
		s_SceneData.CameraDirty = false;
		s_SceneData.LightsDirty = false;
	}
}