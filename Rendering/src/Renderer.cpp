#include "Engine/Rendering/Renderer.h"

#include "Engine/Rendering/MeshGPU.h"
#include "Engine/Rendering/Material.h"
#include "Engine/Rendering/Shader.h"

#include <Engine/Framework/Camera.h>
#include <Engine/Framework/Scene.h>
#include <Engine/Framework/Light.h>
#include <Engine/Framework/Entity.h>

#include <Engine/Core/Application.h>
#include <Engine/Editor/GUI/GUIUtils.h>

#include <glad/glad.h>

namespace Engine::Rendering
{
	Renderer::SceneData Renderer::s_SceneData;

	// MIGHT BE USEFUL LATER
	/*
	*  const glm::vec4& skyboxColor_LDR = glm::vec4(0.420f, 0.710f, 0.850f, 1.0f);
	*  const glm::vec4& skyboxColor_HDR = glm::vec4(0.130f, 0.470f, 0.840f, 1.0f);
	*/

	glm::vec4 Renderer::s_SkyboxColor = glm::vec4(0.130f, 0.470f, 0.840f, 1.0f);

	std::vector<RenderCommand> Renderer::m_CommandQueue;

	std::unique_ptr<Engine::Core::Framebuffer> Renderer::m_ShadowFBO;
	std::unique_ptr<Engine::Core::Framebuffer> Renderer::m_RenderingFBO;

	std::unique_ptr<ShadowPass> Renderer::m_ShadowPass;
	std::unique_ptr<RenderPass> Renderer::m_RenderPass;
	std::unique_ptr<PostProcessPass> Renderer::m_PostProcessPass;

	void Renderer::Init()
	{
		// Gets window size if ImGUI didn't manage to get any size yet
		auto& app = Engine::Core::Application::Get();
		uint32_t initialW = app.GetWindow().GetWindowSize().width;
		uint32_t initialH = app.GetWindow().GetWindowSize().height;

		auto guiSize = Engine::Editor::GUI::GUIUtils::GetViewportSize();
		if (guiSize.x > 0 && guiSize.y > 0) {
			initialW = (uint32_t)guiSize.x;
			initialH = (uint32_t)guiSize.y;
		}

		// FBOS
		{
			// RENDERING FBO
			Engine::Core::Framebuffer::FramebufferSpec renderSpec;
			renderSpec.HasColor = true;
			renderSpec.HDR = true;
			renderSpec.HasDepth = true;
			renderSpec.DepthOnly = false;
			renderSpec.Width = initialW;
			renderSpec.Height = initialH;

			m_RenderingFBO = Engine::Core::Framebuffer::Create(renderSpec);

			// SHADOW FBO
			Engine::Core::Framebuffer::FramebufferSpec shadowSpec;
			shadowSpec.HasColor = false;
			shadowSpec.HDR = false;
			shadowSpec.HasDepth = true;
			shadowSpec.DepthOnly = true;
			shadowSpec.Width = 512;
			shadowSpec.Height = 512;

			m_ShadowFBO = Engine::Core::Framebuffer::Create(shadowSpec);
		}

		// PASSES
		{
			m_RenderPass = Engine::Rendering::RenderPass::Create();
			m_RenderPass->Init(m_RenderingFBO->GetWidth(), m_RenderingFBO->GetHeight());

			m_ShadowPass = Engine::Rendering::ShadowPass::Create();
			m_ShadowPass->Init(m_ShadowFBO->GetWidth(), m_ShadowFBO->GetHeight());

			m_PostProcessPass = Engine::Rendering::PostProcessPass::Create();
			m_PostProcessPass->Init(initialW, initialH);
		}
	}

	void Renderer::OnViewportResize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0) return;

		m_RenderingFBO->Resize(width, height);
		m_RenderPass->Init(width, height);

		if (m_PostProcessPass)
			m_PostProcessPass->Init(width, height);

		//CRTN_LOG_TRACE("[RENDERER]: Viewport resized to {0}x{1}", width, height);
	}

	uint32_t Renderer::GetFinalRenderTexture()
	{
		if (m_PostProcessPass && m_RenderingFBO->GetFramebufferSpec().HDR)
			return m_PostProcessPass->GetColorTexture();

		return m_RenderingFBO->GetColorAttachmentRendererID();
	}

	void Renderer::InitSceneUniforms(const std::shared_ptr<Shader>& shader)
	{
		shader->Bind();

		shader->sceneUniforms.ViewProjection = shader->GetUniformLocation(shader->GetDefaultUniformNames(UniformType::ViewProjection));
		shader->sceneUniforms.ViewPos = shader->GetUniformLocation("u_ViewPos");

		shader->sceneUniforms.SpecularColor = shader->GetUniformLocation("u_SpecularColor");

		shader->sceneUniforms.LightSpaceMatrix = shader->GetUniformLocation("u_LightSpaceMatrix");
		shader->sceneUniforms.ShadowMapTexture = shader->GetUniformLocation("u_ShadowMap");

		shader->sceneUniforms.ShadowType = shader->GetUniformLocation("u_ShadowType");
		shader->sceneUniforms.ShadowBias = shader->GetUniformLocation("u_ShadowBias");
		shader->sceneUniforms.ShadowStrength = shader->GetUniformLocation("u_ShadowStrength");
		s_SceneData.ShadowResolution = m_ShadowFBO->GetWidth();

		shader->sceneUniforms.LightSize = shader->GetUniformLocation("u_LightSize");

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

			shader->DefineUniformInt(shader->sceneUniforms.ShadowType, s.ShadowType);
			shader->DefineUniformFloat(shader->sceneUniforms.ShadowBias, s.ShadowBias);
			shader->DefineUniformFloat(shader->sceneUniforms.ShadowStrength, s.ShadowStrength);
			s_SceneData.ShadowResolution = m_ShadowFBO->GetWidth();

			shader->DefineUniformFloat(shader->sceneUniforms.LightSize, s.LightSize);

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

	void Renderer::SetupCamera(const Engine::Framework::Camera& camera)
	{
		if (camera.IsEnabled() && camera.GetOwner()->IsEnabled())
		{
			const glm::vec3 camPos = camera.GetOwner()->GetTransform().GetPosition();
			const glm::mat4& vp = camera.GetViewProjectionMatrix();
			const float camNear = camera.GetNear();
			const float camFar = camera.GetFar();

			if (s_SceneData.CameraNear != camNear)
			{
				s_SceneData.CameraNear = camNear;
				s_SceneData.CameraDirty = true;
			}

			if (s_SceneData.CameraNear != camFar)
			{
				s_SceneData.CameraFar = camFar;
				s_SceneData.CameraDirty = true;
			}

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
				s_SceneData.LightsDirty = true;
			}

			if (enabled)
			{
				auto& sceneCam = *Engine::Framework::Scene::Get().GetSceneCamera();

				float camNear = sceneCam.GetNear();
				float camFar = sceneCam.GetFar();

				glm::vec3 dir = dirLight.GetDirection();
				glm::vec4 color = dirLight.GetColor();
				float intensity = dirLight.GetIntensity();

				glm::mat4 lightSpaceMatrix = m_ShadowPass->GetDirectionalLightSpaceMatrix();

				int shadowType = dirLight.GetShadowType();
				float shadowBias = dirLight.GetShadowBias();
				float shadowStrength = dirLight.GetShadowStrength();
				float lightSize = dirLight.GetPCSSLightSize();
				float shadowRes = dirLight.GetShadowResolution();
				int renderMode = s_SceneData.RenderMode;

				if (
					s_SceneData.DirLightDirection != dir ||
					s_SceneData.DirLightColor != color ||
					s_SceneData.DirLightIntensity != intensity ||
					s_SceneData.LightSpaceMatrix != lightSpaceMatrix ||
					s_SceneData.ShadowType != shadowType ||
					s_SceneData.ShadowBias != shadowBias ||
					s_SceneData.ShadowStrength != shadowStrength ||
					s_SceneData.ShadowResolution != shadowRes ||
					s_SceneData.LightSize != lightSize ||
					s_SceneData.CameraNear != camNear ||
					s_SceneData.CameraFar != camFar ||
					s_SceneData.RenderMode != renderMode
					)
				{
					s_SceneData.SpecularColor = color;
					s_SceneData.DirLightDirection = dir;
					s_SceneData.DirLightColor = color;
					s_SceneData.DirLightIntensity = intensity;

					m_ShadowPass->CalculateDirectionalLightSpaceMatrix(
						dirLight.GetDirection(),
						sceneCam.GetOwner()->GetTransform().GetPosition(),
						sceneCam.GetNear(),
						sceneCam.GetFar()
					);

					s_SceneData.LightSpaceMatrix = m_ShadowPass->GetDirectionalLightSpaceMatrix();

					s_SceneData.ShadowType = shadowType;
					s_SceneData.ShadowBias = shadowBias;
					s_SceneData.ShadowStrength = shadowStrength;
					s_SceneData.ShadowResolution = shadowRes;
					s_SceneData.LightSize = lightSize;

					auto shadowSpec = m_ShadowFBO->GetFramebufferSpec();
					shadowSpec.Width = shadowRes;
					shadowSpec.Height = shadowRes;

					uint32_t shadowSlot = 1;
					glActiveTexture(GL_TEXTURE0 + shadowSlot);
					glBindTexture(GL_TEXTURE_2D, m_ShadowFBO->GetDepthAttachmentRendererID());

					s_SceneData.ShadowMapTex = shadowSlot;

					m_ShadowFBO->SetFramebufferSpec(shadowSpec);
					m_ShadowFBO->Resize(shadowSpec.Width, shadowSpec.Height);
					m_ShadowPass->Init(shadowSpec.Width, shadowSpec.Height);

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
				auto shadowSpec = m_ShadowFBO->GetFramebufferSpec();
				shadowSpec.Width = src->GetShadowResolution();
				shadowSpec.Height = src->GetShadowResolution();

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

	void Renderer::ExecuteShadowPass()
	{
		if (!m_ShadowPass || !m_ShadowFBO)
		{
			CRTN_LOG_ERROR("[RENDERER]: Shadow pass not initialized!");
			return;
		}

		m_ShadowFBO->Bind();
		glClear(GL_DEPTH_BUFFER_BIT);

		m_ShadowPass->Begin();

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		for (auto& cmd : m_CommandQueue)
			m_ShadowPass->Render(cmd);

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		m_ShadowFBO->Unbind();
		m_ShadowPass->End();
	}

	void Renderer::ExecuteRenderPass()
	{
		if (!m_RenderPass || !m_RenderingFBO)
		{
			CRTN_LOG_ERROR("[RENDERER]: Render pass not initialized!");
			return;
		}

		m_RenderingFBO->Bind();

		// If is on Depth Render Mode, sets skybox color to black for better visualization
		if (s_SceneData.RenderMode == 4)
			glClearColor(0, 0, 0, 0);
		else
			glClearColor(s_SkyboxColor.r, s_SkyboxColor.g, s_SkyboxColor.b, s_SkyboxColor.a);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_RenderPass->Begin();

		for (auto& cmd : m_CommandQueue)
			DrawCommand(cmd);

		m_RenderPass->End();
		m_RenderingFBO->Unbind();
	}

	void Renderer::ExecutePostProcessPass()
	{
		if (!m_PostProcessPass)
		{
			CRTN_LOG_ERROR("[RENDERER]: PostProcess pass not initialized!");
			return;
		}

		m_PostProcessPass->Begin();
		m_PostProcessPass->Render(m_RenderingFBO->GetColorAttachmentRendererID());
		m_PostProcessPass->End();
	}

	void Renderer::EndScene()
	{
		ExecuteShadowPass();
		ExecuteRenderPass();
		ExecutePostProcessPass();

		s_SceneData.CameraDirty = false;
		s_SceneData.LightsDirty = false;
	}

	void Renderer::DrawCommand(const Engine::Rendering::RenderCommand& cmd)
	{
		auto& material = *cmd.MaterialPtr;
		material.Bind();

		auto& shader = material.GetShaderID();

		if (!shader->SceneUniformsInitialized)
		{
			InitSceneUniforms(shader);
			shader->SceneUniformsInitialized = true;
		}

		UploadSceneUniforms(shader);

		glm::mat3 normal = glm::transpose(glm::inverse(glm::mat3(cmd.Transform)));

		shader->DefineUniformMat4(shader->GetUniformLocation(shader->GetDefaultUniformNames(UniformType::Model)), cmd.Transform);
		shader->DefineUniformMat3(shader->GetUniformLocation(shader->GetDefaultUniformNames(UniformType::Normal)), normal);

		shader->DefineUniformInt(shader->GetUniformLocation("u_RenderMode"), s_SceneData.RenderMode);

		int ambient = shader->GetUniformLocation(shader->GetDefaultUniformNames(UniformType::Ambient));
		m_RenderingFBO->GetFramebufferSpec().HDR ? shader->DefineUniformFloat(ambient, 0.1f) : shader->DefineUniformFloat(ambient, 0.3f);

		cmd.MeshPtr->Bind(); // chama m_VAO->Bind();
		cmd.MeshPtr->Draw(); // chama glDrawElements()
	}
}