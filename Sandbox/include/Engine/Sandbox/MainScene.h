#pragma once

#include <Engine/Framework/Scene.h>
#include <Engine/Framework/MeshLibrary.h>
#include <Engine/Rendering/MeshRenderer.h>
#include <Engine/Framework/Light.h>

#include <Engine/Framework/Physics/Collider.h>
#include <Engine/Framework/Physics/PhysicsComponent.h>

#include <glm/gtc/quaternion.hpp>

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

			// SCENE LIGHTS
			{
				auto cyanLightGO = Engine::Framework::Entity::CreateBillboard("[Point Light] New Light", 1.0f, glm::vec4(1.0f));
				cyanLightGO->GetTransform().SetPosition({ 0.0f, 2.0f, 0.0f });

				auto newLight = cyanLightGO->AddComponent<Engine::Framework::Lights::PointLight>();
				newLight->SetIntensity(10.0f);
				newLight->SetColor({ 0.0f, 0.9f, 0.9f, 1.0f }); // cyan
			
				AddEntity(std::move(cyanLightGO));
			}

			// SCENE OBJECTS
			{
				// GROUND
				m_GroundCube = Engine::Framework::Entity::CreateWithCollider("[Entity] Ground Cube", Engine::Framework::MeshLibrary::InstantiateCube());
				m_GroundCube->GetTransform().SetScale({ 10.0f, 0.3f, 10.0f });
				m_GroundCube->GetComponent<Engine::Rendering::MeshRenderer>()->GetMaterial()->SetColor(glm::vec4(0.0f, 0.05f, 1.0f, 1.0f)); // Sets ground color to blue

				AddEntity(std::move(m_GroundCube));

				// DEFAULT CUBE
				m_DefaultCube = Engine::Framework::Entity::CreateWithPhysics("[Entity] Default Cube", Engine::Framework::MeshLibrary::InstantiateCube());
				m_DefaultCube->GetTransform().SetPosition({ 3.0f, 3.0f, 0.0f });
				m_DefaultCube->GetTransform().SetRotation({ 15.0f, 0.0f, 60.0f });
				m_DefaultCube->GetComponent<Engine::Rendering::MeshRenderer>()->GetMaterial()->SetColor(glm::vec4(0.9f, 0.05f, 0.05f, 1.0f)); // Sets color to red

				AddEntity(std::move(m_DefaultCube));

				// SPHERE
				m_DefaultSphere = Engine::Framework::Entity::CreateWithPhysics("[Entity] Default Sphere", Engine::Framework::MeshLibrary::InstantiateSphere());
				m_DefaultSphere->GetTransform().SetPosition({ -3.0f, 3.0f, 0.0f });
				m_DefaultSphere->GetComponent<Engine::Rendering::MeshRenderer>()->GetMaterial()->SetColor(glm::vec4(0.05f, 0.9f, 0.05f, 1.0f)); // Sets color to green

				AddEntity(std::move(m_DefaultSphere));
			}
		}

		Engine::Framework::Physics::PhysicsComponent* GetCubePhysics() {
			if (m_DefaultCube) {
				return m_DefaultCube->GetComponent<Engine::Framework::Physics::PhysicsComponent>();
			}
			return nullptr;
		}
	private:
		std::unique_ptr<Engine::Framework::Entity> m_GroundCube;
		std::unique_ptr<Engine::Framework::Entity> m_DefaultCube;
		std::unique_ptr<Engine::Framework::Entity> m_DefaultSphere;
	};
}