#pragma once

#include <Engine/Framework/Scene.h>
#include <Engine/Framework/MeshLibrary.h>
#include <Engine/Framework/Light.h>

#include <Engine/Framework/Physics/Collider.h>
#include <Engine/Framework/Physics/PhysicsComponent.h>

#include <memory>

namespace Engine::Sandbox
{
	class MainScene : public Engine::Framework::Scene
	{
	public:
		virtual ~MainScene() = default;

		void Init()
		{
			Scene::Init();

			// POINT LIGHTS 
			{
				auto redLight = CreateEntity<Engine::Framework::Lights::PointLight>("[Point Light] Red Light");
				//redLight->SetEnabled(false);
				redLight->GetTransform().SetPosition({ -3.0f, 1.0f, 0.0f });
				redLight->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
				redLight->SetIntensity(10.0f);

				auto greenLight = CreateEntity<Engine::Framework::Lights::PointLight>("[Point Light] Green Light");
				//greenLight->SetEnabled(false);
				greenLight->GetTransform().SetPosition({ 0.0f, 3.0f, 0.0f });
				greenLight->SetColor({ 0.0f, 1.0f, 0.0f, 1.0f });
				greenLight->SetIntensity(10.0f);

				auto blueLight = CreateEntity<Engine::Framework::Lights::PointLight>("[Point Light] Blue Light");
				//blueLight->SetEnabled(false);
				blueLight->GetTransform().SetPosition({ 3.0f, 1.0f, 0.0f });
				blueLight->SetColor({ 0.0f, 0.0f, 1.0f, 1.0f });
				blueLight->SetIntensity(10.0f);

				auto purpleLight = CreateEntity<Engine::Framework::Lights::PointLight>("[Point Light] Purple Light");
				//purpleLight->SetEnabled(false);
				purpleLight->GetTransform().SetPosition({ 0.0f, 1.0f, -3.0f });
				purpleLight->SetColor({ 0.5f, 0.0f, 0.5f, 1.0f });
				purpleLight->SetIntensity(10.0f);

				auto cyanLight = CreateEntity<Engine::Framework::Lights::PointLight>("[Point Light] Cyan Light");
				//cyanLight->SetEnabled(false);
				cyanLight->GetTransform().SetPosition({ 0.0f, 1.0f, 3.0f });
				cyanLight->SetColor({ 0.0f, 0.5f, 0.5f, 1.0f });
				cyanLight->SetIntensity(10.0f);
			}

			// SCENE OBJECTS
			{
				// GROUND
				m_GroundCube = CreateEntity<Engine::Framework::GameObject>("[GameObject] Ground Cube");
				m_GroundCube->AddComponent<Engine::Framework::Physics::CubeCollider>();
				m_GroundCube->AddComponent<Engine::Framework::Physics::PhysicsComponent>();
				m_GroundCube->GetComponent<Engine::Framework::Physics::PhysicsComponent>()->SetStatic(true);

				m_GroundCube->GetTransform().SetPosition({ 0.0f, 0.0f, 0.0f });
				m_GroundCube->GetTransform().SetScale({ 10.0f, 0.3f, 10.0f });

				m_GroundCube->Init();

				m_GroundCube->GetMeshRenderer()->SetMesh(Engine::Framework::MeshLibrary::InstantiateCube());
				m_GroundCube->GetMeshRenderer()->GetMaterial()->SetColor(glm::vec4(0.2f, 0.2f, 0.2f, 1)); // Sets ground color to dark grey

				// DEFAULT CUBE
				m_DefaultCube = CreateEntity<Engine::Framework::GameObject>("[Cube] Default Cube");
				m_DefaultCube->AddComponent<Engine::Framework::Physics::CubeCollider>();
				m_DefaultCube->AddComponent<Engine::Framework::Physics::PhysicsComponent>();
				m_DefaultCube->GetComponent<Engine::Framework::Physics::PhysicsComponent>()->SetStatic(false);
				m_DefaultCube->GetComponent<Engine::Framework::Physics::PhysicsComponent>()->SetMass(200.0f);

				m_DefaultCube->GetTransform().SetPosition({ 0.0f, 5.0f, 0.0f });
				m_DefaultCube->GetTransform().SetRotation({ 45, 45, 45 });
				m_DefaultCube->GetTransform().SetScale({ 1.0f, 1.0f, 1.0f });

				m_DefaultCube->Init();

				m_DefaultCube->GetMeshRenderer()->SetMesh(Engine::Framework::MeshLibrary::InstantiateCube());
				m_DefaultCube->GetMeshRenderer()->GetMaterial()->SetColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)); // Sets cube default color to red
			}
		}
	private:
		std::shared_ptr<Engine::Framework::GameObject> m_GroundCube;
		std::shared_ptr<Engine::Framework::GameObject> m_DefaultCube;
	};
}