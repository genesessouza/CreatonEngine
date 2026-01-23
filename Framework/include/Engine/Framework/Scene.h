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
		virtual ~Scene() = default;

		void Init()
		{
			m_SceneCamera = Engine::Framework::Camera::Create();
			m_DirectionalLight = Engine::Framework::Lights::DirectionalLight::Create();

			AddObject(m_DirectionalLight);
		}

		const void OnViewportResize(uint32_t width, uint32_t height) const
		{
			m_SceneCamera->SetViewportSize(width, height);
		}

		void AddObject(const std::shared_ptr<GameObject>& obj);
		void RemoveObject(const std::shared_ptr<GameObject>& obj);

		void Render();

		const std::shared_ptr<Engine::Framework::Lights::DirectionalLight>& GetDirectionalLight() const { return m_DirectionalLight; }
		const std::vector<Engine::Framework::Lights::PointLight*>& GetPointLights() const { return m_PointLights; }

		const std::vector<std::shared_ptr<GameObject>> GetObjects() const { return m_SceneObjects; }
	public:
		std::shared_ptr<Camera> m_SceneCamera;
	private:
		std::shared_ptr<Engine::Framework::Lights::DirectionalLight> m_DirectionalLight;
		std::vector<Engine::Framework::Lights::PointLight*> m_PointLights;

		std::vector<std::shared_ptr<GameObject>> m_SceneObjects;
	};
}