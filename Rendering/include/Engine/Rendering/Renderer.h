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
			uint32_t ShadowMapTex;
			glm::mat4 LightSpaceMatrix;
			glm::mat4 ViewProjection;

			glm::vec3 ViewPos;

			glm::vec4 SpecularColor;

			// Directional Light
			glm::vec3 DirLightDirection;
			glm::vec4 DirLightColor;
			float DirLightIntensity;

			bool HasDirLight = false;

			// Point Lights
			static constexpr uint32_t MaxPointLights = 8;
			uint32_t PointLightCount = 0;
			PointLightData PointLights[MaxPointLights];

			// Dirty flags
			bool CameraDirty = true;
			bool LightsDirty = true;
		};
	public:
		Renderer() = default;
		virtual ~Renderer() = default;

		static void Init();
		static void InitSceneUniforms(const std::shared_ptr<Shader>& shader);
		static void UploadSceneUniforms(const std::shared_ptr<Shader>& shader);

		static void MarkCameraDirty() { s_SceneData.CameraDirty = true; }
		static void MarkLightsDirty() { s_SceneData.LightsDirty = true; }

		static void SetClearColor(const glm::vec4& color);

		static void SetupCamera(const Engine::Framework::Camera& camera);
		static void SetupLights(const Engine::Framework::Scene& scene);

		static void BeginScene(const Engine::Framework::Camera& camera, const Engine::Framework::Scene& scene);

		static void Submit(Engine::Rendering::MeshGPU* mesh, Engine::Rendering::Material* mat, const glm::mat4& transformMatrix);
		static void RunShadowPass();
		static void EndScene();		

		static const SceneData& GetSceneData() { return s_SceneData; }
	private:
		static SceneData s_SceneData;
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