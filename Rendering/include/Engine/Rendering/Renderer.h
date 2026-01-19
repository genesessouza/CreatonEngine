#pragma once

#include <Engine/Framework/Light.h>
#include <Engine/Framework/Camera.h>

#include <Engine/Framework/GameObject.h>

#include <memory>

namespace Engine::Rendering
{
	struct PointLightData
	{
		glm::vec3 Position;
		glm::vec4 Color;

		float Intensity;
	};

	class Renderer
	{
	public:
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
		};
	public:
		static void BeginScene(std::shared_ptr<Framework::Camera>& camera, const std::vector<std::shared_ptr<Framework::GameObject>>& sceneObjects);
		static void EndScene();

		static const SceneData& GetSceneData() { return s_SceneData; }
	private:
		static SceneData s_SceneData;
	};
}