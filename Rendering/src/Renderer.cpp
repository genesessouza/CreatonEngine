#include "Engine/Rendering/Renderer.h"

#include "Engine/Rendering/MeshGPU.h"
#include "Engine/Rendering/Material.h"
#include "Engine/Rendering/Shader.h"

#include <Engine/Framework/Camera.h>
#include <Engine/Framework/Scene.h>
#include <Engine/Framework/Light.h>
#include <Engine/Framework/Entity.h>

#include <Engine/Core/Application.h>

#include <glad/glad.h>

namespace Engine::Rendering
{
	Renderer::SceneData Renderer::s_SceneData;
	std::vector<RenderCommand> Renderer::m_CommandQueue;

	std::unique_ptr<Engine::Core::Framebuffer> Renderer::m_ShadowFBO;
	std::unique_ptr<Engine::Core::Framebuffer> Renderer::m_RenderingFBO;

	std::unique_ptr<ShadowPass> Renderer::m_ShadowPass;
	std::unique_ptr<RenderPass> Renderer::m_RenderPass;
	std::unique_ptr<PostProcessPass> Renderer::m_PostProcessPass;

	void Renderer::Init()
	{
		// FBOS
		{
			// RENDERING FBO
			Engine::Core::Framebuffer::FramebufferSpec renderSpec;
			renderSpec.HDR = true;
			renderSpec.Width = 1280;
			renderSpec.Height = 720;
			m_RenderingFBO = Engine::Core::Framebuffer::Create(renderSpec);

			// SHADOW FBO
			Engine::Core::Framebuffer::FramebufferSpec shadowSpec;
			shadowSpec.DepthOnly = true;
			shadowSpec.Width = 2048;
			shadowSpec.Height = 2048;
			m_ShadowFBO = Engine::Core::Framebuffer::Create(shadowSpec);
		}

		// PASSES
		{
			m_RenderPass = Engine::Rendering::RenderPass::Create();
			m_RenderPass->Init();

			m_ShadowPass = Engine::Rendering::ShadowPass::Create();
			m_ShadowPass->Init(2048, 2048);

			m_PostProcessPass = Engine::Rendering::PostProcessPass::Create();
			m_PostProcessPass->Init(1280, 720);
		}
	}

	void Renderer::InitSceneUniforms(const std::shared_ptr<Shader>& shader)
	{
		shader->Bind();

		shader->sceneUniforms.ViewProjection = shader->GetUniformLocation(shader->GetDefaultUniformNames(UniformType::ViewProjection));
		shader->sceneUniforms.ViewPos = shader->GetUniformLocation("u_ViewPos");

		shader->sceneUniforms.SpecularColor = shader->GetUniformLocation("u_SpecularColor");

		shader->sceneUniforms.LightSpaceMatrix = shader->GetUniformLocation("u_LightSpaceMatrix");
		shader->sceneUniforms.ShadowMapTexture = shader->GetUniformLocation("u_ShadowMap");

		shader->sceneUniforms.HasDirLight = shader->GetUniformLocation("u_HasDirLight");
		shader->sceneUniforms.DirLightDir = shader->GetUniformLocation("u_DirLightDir");
		shader->sceneUniforms.DirLightColor = shader->GetUniformLocation("u_DirLightColor");
		shader->sceneUniforms.DirLightIntensity = shader->GetUniformLocation("u_LightIntensity");

		shader->sceneUniforms.PointLightCount = shader->GetUniformLocation("u_PointLightCount");

		for (int i = 0; i < 8; i++)
		{
			shader->sceneUniforms.PointLightPos[i] = shader->GetUniformLocation("u_PointLights[" + std::to_string(i) + "].Position");
			shader->sceneUniforms.PointLightColor[i] = shader->GetUniformLocation("u_PointLights[" + std::to_string(i) + "].Color");
			shader->sceneUniforms.PointLightIntensity[i] = shader->GetUniformLocation("u_PointLights[" + std::to_string(i) + "].Intensity");
		}
	}

	void Renderer::UploadSceneUniforms(const std::shared_ptr<Shader>& shader)
	{
		const auto& s = s_SceneData;

		if (s.CameraDirty)
		{
			shader->DefineUniformMat4(shader->sceneUniforms.ViewProjection, s.ViewProjection);
			shader->DefineUniformVec3(shader->sceneUniforms.ViewPos, s.ViewPos);
		}

		if (s.LightsDirty)
		{
			shader->DefineUniformInt(shader->sceneUniforms.ShadowMapTexture, s.ShadowMapTex);
			shader->DefineUniformMat4(shader->sceneUniforms.LightSpaceMatrix, s.LightSpaceMatrix);
			shader->DefineUniformBool(shader->sceneUniforms.HasDirLight, s.HasDirLight);

			shader->DefineUniformVec4(shader->sceneUniforms.SpecularColor, s.SpecularColor);

			if (s.HasDirLight)
			{
				shader->DefineUniformVec3(shader->sceneUniforms.DirLightDir, s.DirLightDirection);
				shader->DefineUniformVec4(shader->sceneUniforms.DirLightColor, s.DirLightColor);
				shader->DefineUniformFloat(shader->sceneUniforms.DirLightIntensity, s.DirLightIntensity);
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

	void Renderer::SetupCamera(const Engine::Framework::Camera& camera)
	{
		if (camera.IsEnabled() && camera.GetOwner()->IsEnabled())
		{
			const glm::vec3 camPos = camera.GetOwner()->GetTransform().GetPosition();
			const glm::mat4& vp = camera.GetViewProjectionMatrix();

			if (s_SceneData.ViewPos != camPos)
			{
				s_SceneData.ViewPos = camPos;
				s_SceneData.CameraDirty = true;
			}

			if (camera.GetOwner()->GetTransform().WasModifiedThisFrame())
			{
				s_SceneData.ViewProjection = vp;
				s_SceneData.CameraDirty = true;
			}
		}
	}

	void Renderer::SetupLights(const Engine::Framework::Scene& scene)
	{
		bool lightsChanged = false;

		// --- DIRECTIONAL LIGHT ---
		auto dirLightPtr = scene.GetDirectionalLight();
		if (dirLightPtr)
		{
			auto& dirLight = *dirLightPtr;
			bool enabled = dirLight.IsEnabled() && dirLight.GetOwner()->IsEnabled();

			if (s_SceneData.HasDirLight != enabled)
			{
				s_SceneData.HasDirLight = enabled;
				lightsChanged = true;
			}

			if (enabled)
			{
				m_ShadowPass->UpdateLightMatrix(dirLight.GetDirection());
				s_SceneData.LightSpaceMatrix = m_ShadowPass->GetLightSpaceMatrix();

				glActiveTexture(GL_TEXTURE5);
				glBindTexture(GL_TEXTURE_2D, m_ShadowPass->GetDepthTexture());
				s_SceneData.ShadowMapTex = 5;

				glm::vec3 dir = dirLight.GetDirection();
				glm::vec4 color = dirLight.GetColor();
				float intensity = dirLight.GetIntensity();

				if (s_SceneData.DirLightDirection != dir || s_SceneData.DirLightColor != color || s_SceneData.DirLightIntensity != intensity)
				{
					s_SceneData.SpecularColor = color;
					s_SceneData.DirLightDirection = dir;
					s_SceneData.DirLightColor = color;
					s_SceneData.DirLightIntensity = intensity;
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

			auto pLightEnabled = src->IsEnabled() && src->GetOwner()->IsEnabled();
			auto& dst = s_SceneData.PointLights[i];

			const glm::vec3 pos = src->GetOwner()->GetTransform().GetPosition();
			const glm::vec4 color = src->GetColor();
			const float intensity = src->GetIntensity();

			if (pLightEnabled)
			{
				if (dst.Position != pos || dst.Color != color || dst.Intensity != intensity)
				{
					dst.Position = pos;

					dst.Color = color;
					s_SceneData.SpecularColor = color;

					dst.Intensity = intensity;
					lightsChanged = true;
				}
			}
			else
			{
				dst.Position = glm::vec3(0.0f);
				dst.Color = glm::vec4(0.0f);
				dst.Intensity = 0.0f;
				lightsChanged = true;
			}
		}

		if (lightsChanged)
			s_SceneData.LightsDirty = true;
	}

	void Renderer::BeginScene(const Framework::Camera& camera, const Engine::Framework::Scene& scene)
	{
		m_CommandQueue.clear();

		SetupCamera(camera);
		SetupLights(scene);
	}

	void Renderer::Submit(Engine::Rendering::MeshGPU* mesh, Engine::Rendering::Material* mat, const glm::mat4& transformMatrix)
	{
		m_CommandQueue.push_back({ mesh, mat, transformMatrix });
	}

	void Renderer::RunShadowPass()
	{
		// SHADOW PASS
		m_ShadowPass->Begin();
		m_ShadowFBO->Bind();

		glClear(GL_DEPTH_BUFFER_BIT);

		s_SceneData.LightSpaceMatrix = m_ShadowPass->GetLightSpaceMatrix();

		for (auto& cmd : m_CommandQueue)
			m_ShadowPass->Render(cmd);

		m_ShadowFBO->Unbind();
		m_ShadowPass->End();
	}

	void Renderer::EndScene()
	{
		RunShadowPass();

		m_RenderingFBO->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto& cmd : m_CommandQueue)
			DrawCommand(cmd);

		m_RenderingFBO->Unbind();

		m_PostProcessPass->Begin();
		m_PostProcessPass->Render(m_RenderingFBO->GetColorAttachmentRendererID());
		m_PostProcessPass->End();

		s_SceneData.CameraDirty = false;
		s_SceneData.LightsDirty = false;
	}

	void Renderer::DrawCommand(const Engine::Rendering::RenderCommand& cmd)
	{
		auto& material = *cmd.MaterialPtr;
		material.Bind();

		auto& shader = material.GetShaderID();

		UploadSceneUniforms(shader);

		glm::mat3 normal = glm::transpose(glm::inverse(glm::mat3(cmd.Transform)));

		shader->DefineUniformMat4(shader->GetUniformLocation(shader->GetDefaultUniformNames(UniformType::Model)), cmd.Transform);
		shader->DefineUniformMat3(shader->GetUniformLocation(shader->GetDefaultUniformNames(UniformType::Normal)), normal);

		cmd.MeshPtr->Bind(); // chama m_VAO->Bind();
		cmd.MeshPtr->Draw(); // chama glDrawElements()
	}
}