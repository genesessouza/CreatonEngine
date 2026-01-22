#pragma once

#include <Engine/Framework/Scene.h>
#include <Engine/Framework/MeshLibrary.h>
#include <Engine/Framework/Light.h>

#include <memory>

namespace Engine::Sandbox
{
	class MainScene
		: public Engine::Framework::Scene
	{
	public:
		MainScene()
			: Engine::Framework::Scene()
		{
		}

		void Init()
		{
			Scene::Init();
			
			// POINT LIGHTS 
			{
				auto redLight = Engine::Framework::Lights::PointLight::Create();
				redLight->m_Transform.SetPosition({ -3.0f, 1.0f, 0.0f });
				redLight->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
				redLight->SetIntensity(10.0f);

				//AddObject(redLight);

				auto greenLight = Engine::Framework::Lights::PointLight::Create();
				greenLight->m_Transform.SetPosition({ 0.0f, 3.0f, 0.0f });
				greenLight->SetColor({ 0.0f, 1.0f, 0.0f, 1.0f });
				greenLight->SetIntensity(10.0f);

				//AddObject(greenLight);

				auto blueLight = Engine::Framework::Lights::PointLight::Create();
				blueLight->m_Transform.SetPosition({ 3.0f, 1.0f, 0.0f });
				blueLight->SetColor({ 0.0f, 0.0f, 1.0f, 1.0f });
				blueLight->SetIntensity(10.0f);

				//AddObject(blueLight);

				auto purpleLight = Engine::Framework::Lights::PointLight::Create();
				purpleLight->m_Transform.SetPosition({ 0.0f, 1.0f, -3.0f });
				purpleLight->SetColor({ 0.5f, 0.0f, 0.5f, 1.0f });
				purpleLight->SetIntensity(10.0f);

				//AddObject(purpleLight);

				auto cyanLight = Engine::Framework::Lights::PointLight::Create();
				cyanLight->m_Transform.SetPosition({ 0.0f, 1.0f, 3.0f });
				cyanLight->SetColor({ 0.0f, 0.5f, 0.5f, 1.0f });
				cyanLight->SetIntensity(10.0f);

				//AddObject(cyanLight);
			}

			m_SceneCamera->MoveCamera({ 0, 2, -9 });
			//m_SceneCamera->MoveCamera({ 0, 10, 0 });
			//m_SceneCamera->RotateCamera({ 90, 0, 0 });

			// GROUND
			m_GroundCube = Engine::Framework::GameObject::Create();
			m_GroundCube->Init();

			m_GroundCube->m_Transform.SetPosition({ 0.0f, 0.0f, 0.0f });
			m_GroundCube->m_Transform.SetScale({ 10.0f, 0.3f, 10.0f });

			m_GroundCube->m_MeshRenderer->SetMesh(Engine::Framework::MeshLibrary::InstantiateCube());
			m_GroundCube->m_MeshRenderer->GetMaterial()->SetColor(glm::vec4(0.2f, 0.2f, 0.2f, 1), 1.0f); // Sets ground color to dark grey

			AddObject(m_GroundCube);

			// DEFAULT CUBE
			m_DefaultCube = Engine::Framework::GameObject::Create();
			m_DefaultCube->Init();

			m_DefaultCube->m_Transform.SetPosition({ 0.0f, 1.0f, 0.0f });
			m_DefaultCube->m_Transform.SetScale({ 1.0f, 1.0f, 1.0f });

			m_DefaultCube->m_MeshRenderer->SetMesh(Engine::Framework::MeshLibrary::InstantiateCube());
			m_DefaultCube->m_MeshRenderer->GetMaterial()->SetColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), 1.0f); // Sets cube default color to red

			AddObject(m_DefaultCube);
		}
	private:
		std::shared_ptr<Engine::Framework::GameObject> m_GroundCube;
		std::shared_ptr<Engine::Framework::GameObject> m_DefaultCube;
	};
}