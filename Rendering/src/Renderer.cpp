#include "Engine/Rendering/Renderer.h"

#include "Engine/Rendering/Shader.h"

#include <Engine/Framework/Camera.h>
#include <Engine/Framework/Scene.h>
#include <Engine/Framework/Light.h>
#include <Engine/Framework/GameObject.h>

#include <glad/glad.h>

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
		const auto& s = s_SceneData;

		if (s.CameraDirty)
		{
			shader->DefineUniformMat4(shader->sceneUniforms.ViewProjection, s.ViewProjection);
			shader->DefineUniformVec3(shader->sceneUniforms.ViewPos, s.ViewPos);
		}

		if (s.LightsDirty)
		{
			shader->DefineUniformBool(shader->sceneUniforms.HasDirLight, s.HasDirLight);

			if (s.HasDirLight)
			{
				shader->DefineUniformVec3(shader->sceneUniforms.DirLightDir, s.DirLightDirection);
				shader->DefineUniformVec4(shader->sceneUniforms.DirLightColor, s.DirLightColor);
			}

			shader->DefineUniformInt(shader->sceneUniforms.PointLightCount, s.PointLightCount);

			for (uint32_t i = 0; i < s.PointLightCount; ++i)
			{
				const auto& l = s.PointLights[i];
				shader->DefineUniformVec3(shader->sceneUniforms.PointLightPos[i], l.Position);
				shader->DefineUniformVec4(shader->sceneUniforms.PointLightColor[i], l.Color);
				shader->DefineUniformFloat(shader->sceneUniforms.PointLightIntensity[i], l.Intensity);
			}
		}
	}

	void Renderer::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::BeginScene(const Framework::Camera& camera, const Engine::Framework::Scene& scene)
	{
		// --- CAMERA ---
		const glm::vec3 camPos = camera.GetTransform().GetPosition();
		const glm::mat4& vp = camera.GetViewProjectionMatrix();

		if (s_SceneData.ViewPos != camPos)
		{
			s_SceneData.ViewPos = camPos;
			s_SceneData.CameraDirty = true;
		}

		if (camera.GetTransform().WasModifiedThisFrame())
		{
			s_SceneData.ViewProjection = vp;
			s_SceneData.CameraDirty = true;
		}

		// --- DIRECTIONAL LIGHT ---
		bool lightsChanged = false;

		auto& dirLight = *scene.GetDirectionalLight();

		if(&dirLight)
		{
			bool enabled = dirLight.IsEnabled();

			if (s_SceneData.HasDirLight != enabled)
			{
				s_SceneData.HasDirLight = enabled;
				lightsChanged = true;
			}

			if (enabled)
			{
				glm::vec3 dir = dirLight.GetDirection();
				glm::vec4 color = dirLight.GetColor();

				if (s_SceneData.DirLightDirection != dir ||
					s_SceneData.DirLightColor != color)
				{
					s_SceneData.DirLightDirection = dir;
					s_SceneData.DirLightColor = color;
					lightsChanged = true;
				}
			}
		}
		else if (s_SceneData.HasDirLight)
		{
			s_SceneData.HasDirLight = false;
			lightsChanged = true;
		}

		// --- POINT LIGHTS ---
		const auto& scenePointLights = scene.GetPointLights();
		const uint32_t count = std::min<uint32_t>(scenePointLights.size(), SceneData::MaxPointLights);

		if (s_SceneData.PointLightCount != count)
		{
			s_SceneData.PointLightCount = count;
			lightsChanged = true;
		}

		for (uint32_t i = 0; i < count; ++i)
		{
			auto& src = scenePointLights[i];
			auto& dst = s_SceneData.PointLights[i];

			const glm::vec3 pos = src->GetOwner()->GetTransform().GetPosition();
			const glm::vec4 col = src->GetColor();
			const float intensity = src->GetIntensity();

			if (dst.Position != pos ||
				dst.Color != col ||
				dst.Intensity != intensity)
			{
				dst.Position = pos;
				dst.Color = col;
				dst.Intensity = intensity;
				lightsChanged = true;
			}
		}

		if (lightsChanged)
			s_SceneData.LightsDirty = true;
	}

	void Renderer::EndScene()
	{
		s_SceneData.CameraDirty = false;
		s_SceneData.LightsDirty = false;
	}
}