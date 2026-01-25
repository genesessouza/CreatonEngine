#pragma once

#include "Engine/Rendering/Shader.h"

#include <glm/glm.hpp>

#include <memory>

namespace Engine::Framework {
	class Scene;
	class Camera;
}

namespace Engine::Rendering
{
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
			bool HasDirLight = false;

			// Point Lights
			std::vector<PointLightData> PointLights;

			// Dirty flags
			bool CameraDirty = true;
			bool LightsDirty = true;
		};
	public:
		static void InitSceneUniforms(const std::shared_ptr<Engine::Rendering::Shader>& shader);
		static void UploadSceneUniforms(const std::shared_ptr<Engine::Rendering::Shader>& shader);

		static void MarkCameraDirty() { s_SceneData.CameraDirty = true; }
		static void MarkLightsDirty() { s_SceneData.LightsDirty = true; }

		static void SetClearColor(const glm::vec4& color);

		static void BeginScene(std::shared_ptr<Engine::Framework::Camera>& camera, const Engine::Framework::Scene& scene);
		static void EndScene();

		static const SceneData& GetSceneData() { return s_SceneData; }
	private:
		static SceneData s_SceneData;
	};
}