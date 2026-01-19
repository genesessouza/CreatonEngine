#pragma once

#include <Engine/Core/Log/Logger.h>

#include "Engine/Framework/Camera.h"
#include "Engine/Framework/GameObject.h"
#include "Engine/Framework/Light.h"

#include <Engine/Rendering/Renderer.h>

#include <glad/glad.h>

namespace Engine::Framework
{
	class Scene
	{
	public:
		Scene()
		{
			glClearColor(0.3f, 0.7f, 0.9f, 1.0f);

			CRTN_LOG_INFO("[SCENE]: Background color set to: <Sky Blue>\n");
		}

		virtual ~Scene() = default;

		void Init()
		{
			m_SceneCamera = Engine::Framework::Camera::Create();
			m_DirectionalLight = Engine::Framework::Lights::DirectionalLight::Create();

			m_SceneObjects.push_back(m_DirectionalLight);
		}

		const void OnViewportResize(uint32_t width, uint32_t height) const
		{
			m_SceneCamera->SetViewportSize(width, height);
		}

		virtual void Render()
		{
			Engine::Rendering::Renderer::BeginScene(m_SceneCamera, m_SceneObjects);

			for (auto& obj : m_SceneObjects)
				obj->Draw();

			Engine::Rendering::Renderer::EndScene();
		}

	public:
		std::shared_ptr<Camera> m_SceneCamera;
		std::shared_ptr<Engine::Framework::Lights::DirectionalLight> m_DirectionalLight;
		std::vector<std::shared_ptr<GameObject>> m_SceneObjects;
	};
}