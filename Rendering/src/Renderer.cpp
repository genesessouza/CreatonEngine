#include "Engine/Rendering/Renderer.h"

namespace Engine::Rendering
{
	Renderer::SceneData Renderer::s_SceneData;

	void Renderer::BeginScene(std::shared_ptr<Framework::Camera>& camera, const std::vector<std::shared_ptr<Framework::GameObject>>& sceneObjects)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		CRTN_CHECK_PTR(camera);
		s_SceneData.ViewProjection = camera->GetViewProjectionMatrix();
		s_SceneData.ViewPos = camera->GetPosition();

		s_SceneData.PointLights.clear();
		s_SceneData.HasDirLight = false;

		for (const auto& obj : sceneObjects)
		{
			auto dirLight = std::dynamic_pointer_cast<Engine::Framework::Lights::DirectionalLight>(obj);
			//CRTN_CHECK_PTR(dirLight);

			if (dirLight)
			{
				s_SceneData.DirLightDirection = dirLight->GetDirection();
				s_SceneData.DirLightColor = dirLight->GetColor();
				s_SceneData.HasDirLight = true;
				continue;
			}

			auto pointLight = std::dynamic_pointer_cast<Engine::Framework::Lights::PointLight>(obj);
			//CRTN_CHECK_PTR(pointLight);

			if (pointLight && s_SceneData.PointLights.size() < 8)
				s_SceneData.PointLights.push_back({ obj->m_Transform.GetPosition(), pointLight->GetColor(), pointLight->GetIntensity() });
		}
	}

	void Renderer::EndScene()
	{
	}
}