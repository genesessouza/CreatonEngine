#pragma once

#include <glm/glm.hpp>

#include <memory>

namespace Engine::Framework
{
	class Camera;
	class Scene;
	class Entity;
}

namespace Engine::Framework::Lights
{
	class PointLight;
	class DirectionalLight;
}

namespace Engine::Rendering
{
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
			glm::mat4 ViewProjection;

			glm::vec3 ViewPos;
			
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
		static void InitSceneUniforms(const std::shared_ptr<Shader>& shader);
		static void UploadSceneUniforms(const std::shared_ptr<Shader>& shader);

		static void MarkCameraDirty() { s_SceneData.CameraDirty = true; }
		static void MarkLightsDirty() { s_SceneData.LightsDirty = true; }

		static void SetClearColor(const glm::vec4& color);

		static void BeginScene(const Engine::Framework::Camera& camera, const Engine::Framework::Scene& scene);
		static void EndScene();

		static const SceneData& GetSceneData() { return s_SceneData; }
	private:
		static SceneData s_SceneData;
	};
}