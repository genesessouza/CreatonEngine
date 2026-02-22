#pragma once

#include <Engine/Core/Framebuffer.h>

#include "Engine/Rendering/RenderPass.h"
#include "Engine/Rendering/RenderCommand.h"

#include <glm/glm.hpp>

#include <memory>

namespace Engine::Framework
{
	class Camera;
	class Scene;
	class Entity;

	namespace Lights
	{
		class PointLight;
		class DirectionalLight;
	}
}

namespace Engine::Rendering
{
	class MeshGPU;
	class Material;
	class Shader;

	class Renderer
	{
	public:
		struct PointLightData
		{
			glm::vec3 Position;
			glm::vec4 Color;

			float Intensity;
		};
		struct SceneData
		{
			/* 
			*  ---- RENDER MODES ----
			*  0 - Ambient
			*  1 - Diffuse
			*  2 - Specular
			*  3 - Normals
			*  4 - Depth
			*  5 - Lit (Blinn-Phong + Shadows)
			*/
			uint32_t RenderMode = 5;

			glm::mat4 LightSpaceMatrix;
			glm::mat4 ViewProjection;

			glm::vec3 ViewPos;
			float CameraNear = 0.1f;
			float CameraFar = 100.0f;

			glm::vec4 SpecularColor;

			int ShadowType = 0;
			float ShadowBias = 0.0005f;
			float ShadowStrength = 0.9f;
			uint32_t ShadowResolution = 256.0f;
			uint32_t ShadowMapTex;

			// Directional Light
			float LightSize = 0.01f;
			glm::vec3 DirLightDirection;
			glm::vec4 DirLightColor;
			float DirLightIntensity;

			bool HasDirLight = false;

			// Point Lights
			static constexpr uint32_t MaxPointLights = 8;
			uint32_t PointLightCount = 0;
			PointLightData PointLights[MaxPointLights];

			bool RenderShadows = false;

			// Dirty flags
			bool CameraDirty = true;
			bool LightsDirty = true;
			bool ShadowsDirty = true;
		};
	public:
		Renderer() = default;
		virtual ~Renderer() = default;

		static void Init();
		static void OnViewportResize(uint32_t width, uint32_t height);
		static uint32_t GetFinalRenderTexture();

		static void InitSceneUniforms(const std::shared_ptr<Shader>& shader);
		static void UploadSceneUniforms(const std::shared_ptr<Shader>& shader);

		static void MarkCameraDirty() { s_SceneData.CameraDirty = true; }
		static void MarkLightsDirty() { s_SceneData.LightsDirty = true; }

		static void SetRenderMode(int mode) { s_SceneData.RenderMode = mode; }
		static int GetRenderMode() { return s_SceneData.RenderMode; }

		static const glm::vec4& GetSkyboxColor() { return s_SkyboxColor; }
		static void SetSkyboxColor(const glm::vec4& color) { s_SkyboxColor = color; }

		static void SetupCamera(const Engine::Framework::Camera& camera);
		static void SetupLights(const Engine::Framework::Scene& scene);

		static void BeginScene(const Engine::Framework::Camera& camera, const Engine::Framework::Scene& scene);

		static void Submit(Engine::Rendering::MeshGPU* mesh, Engine::Rendering::Material* mat, const glm::mat4& transformMatrix);
		static void ExecuteShadowPass();
		static void ExecuteRenderPass();
		static void ExecutePostProcessPass();
		static void EndScene();

		static const SceneData& GetSceneData() { return s_SceneData; }

		static Engine::Core::Framebuffer& GetRenderingFBO() { return *m_RenderingFBO.get(); }

		static Engine::Core::Framebuffer* GetShadowFBO() { return m_ShadowFBO.get(); }

		static ShadowPass* GetShadowPass() { return m_ShadowPass.get(); }
		static RenderPass* GetRenderPass() { return m_RenderPass.get(); }
		static PostProcessPass* GetPostProcessPass() { return m_PostProcessPass.get(); }
	private:
		static SceneData s_SceneData;
		static glm::vec4 s_SkyboxColor;
	private:
		static std::unique_ptr<Engine::Core::Framebuffer> m_RenderingFBO;
		static std::unique_ptr<Engine::Core::Framebuffer> m_ShadowFBO;

		static std::unique_ptr<ShadowPass> m_ShadowPass;
		static std::unique_ptr<RenderPass> m_RenderPass;
		static std::unique_ptr<PostProcessPass> m_PostProcessPass;
	private:
		static void DrawCommand(const RenderCommand& cmd);
		static std::vector<RenderCommand> m_CommandQueue;
	};
}