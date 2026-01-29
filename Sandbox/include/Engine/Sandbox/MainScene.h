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

			// POINT LIGHTS 
			/*{
				auto redLightGO = Engine::Framework::GameObject::Create("[Point Light] Red Light");
				redLightGO->GetTransform().SetPosition({ -3.0f, 1.0f, 0.0f });
				redLightGO->GetTransform().SetRotation(glm::quat(glm::vec3(0.0f)));

				redLightGO->AddComponent<Engine::Rendering::MeshRenderer>();
				
				auto redLightRenderer = redLightGO->GetComponent<Engine::Rendering::MeshRenderer>();
				redLightRenderer->Init();
				redLightRenderer->GetMaterial()->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });

				AddEntity(std::move(redLightGO));
				AddRenderable(redLightRenderer);

				auto greenLight = Engine::Framework::Lights::PointLight::Create("[Point Light] Green Light");
				//greenLight->SetEnabled(false);
				greenLight->GetTransform().SetPosition({ 0.0f, 3.0f, 0.0f });
				greenLight->SetColor({ 0.0f, 1.0f, 0.0f, 1.0f });
				greenLight->SetIntensity(10.0f);

				AddPointLight(greenLight.get());

				auto blueLight = Engine::Framework::Lights::PointLight::Create("[Point Light] Blue Light");
				//blueLight->SetEnabled(false);
				blueLight->GetTransform().SetPosition({ 3.0f, 1.0f, 0.0f });
				blueLight->SetColor({ 0.0f, 0.0f, 1.0f, 1.0f });
				blueLight->SetIntensity(10.0f);

				AddPointLight(blueLight.get());

				auto purpleLight = Engine::Framework::Lights::PointLight::Create("[Point Light] Purple Light");
				//purpleLight->SetEnabled(false);
				purpleLight->GetTransform().SetPosition({ 0.0f, 1.0f, -3.0f });
				purpleLight->SetColor({ 0.5f, 0.0f, 0.5f, 1.0f });
				purpleLight->SetIntensity(10.0f);

				AddPointLight(purpleLight.get());

				auto cyanLight = Engine::Framework::Lights::PointLight::Create("[Point Light] Cyan Light");
				//cyanLight->SetEnabled(false);
				cyanLight->GetTransform().SetPosition({ 0.0f, 1.0f, 3.0f });
				cyanLight->SetColor({ 0.0f, 0.5f, 0.5f, 1.0f });
				cyanLight->SetIntensity(10.0f);

				AddPointLight(cyanLight.get());
			}*/

			// SCENE OBJECTS
			{
				// GROUND
				m_GroundCube = Engine::Framework::GameObject::Create("[GameObject] Ground Cube");
				m_GroundCube->AddComponent<Engine::Framework::Physics::CubeCollider>(glm::vec3(1.0f));
				m_GroundCube->AddComponent<Engine::Rendering::MeshRenderer>();
				m_GroundCube->AddComponent<Engine::Framework::Physics::PhysicsComponent>();
				m_GroundCube->GetTransform().SetPosition({ 0.0f, 0.0f, 0.0f });
				m_GroundCube->GetTransform().SetScale({ 10.0f, 0.3f, 10.0f });

				auto groundCollider = m_GroundCube->GetComponent<Engine::Framework::Physics::Collider>();
				groundCollider->SetOwner(m_GroundCube.get());
				AddCollider(groundCollider);

				auto groundRenderer = m_GroundCube->GetComponent<Engine::Rendering::MeshRenderer>();
				groundRenderer->SetOwner(m_GroundCube.get());
				groundRenderer->Init();
				groundRenderer->SetMesh(Engine::Framework::MeshLibrary::InstantiateCube());
				groundRenderer->GetMaterial()->SetColor(glm::vec4(0.2f, 0.2f, 0.2f, 1)); // Sets ground color to dark grey
				AddRenderable(groundRenderer);

				auto groundPhysics = m_GroundCube->GetComponent<Engine::Framework::Physics::PhysicsComponent>();
				groundPhysics->SetOwner(m_GroundCube.get());
				groundPhysics->SetStatic(true);
				AddPhysicsComponent(groundPhysics);

				AddEntity(std::move(m_GroundCube));

				//m_GroundCube->Init();

				// DEFAULT CUBE
				m_DefaultCube = Engine::Framework::GameObject::Create("[GameObject] Default Cube");
				m_DefaultCube->AddComponent<Engine::Framework::Physics::CubeCollider>(glm::vec3(1.0f));
				m_DefaultCube->AddComponent<Engine::Rendering::MeshRenderer>();
				m_DefaultCube->AddComponent<Engine::Framework::Physics::PhysicsComponent>();

				m_DefaultCube->GetTransform().SetPosition({ 0.0f, 5.0f, 0.0f });
				m_DefaultCube->GetTransform().SetRotation(glm::quat(glm::vec3(15.0f, 0.0f, 60.0f)));
				m_DefaultCube->GetTransform().SetScale({ 1.0f, 1.0f, 1.0f });

				auto defaultCubeColl = m_DefaultCube->GetComponent<Engine::Framework::Physics::Collider>();
				defaultCubeColl->SetOwner(m_DefaultCube.get());
				AddCollider(defaultCubeColl);

				auto defaultCubeRen = m_DefaultCube->GetComponent<Engine::Rendering::MeshRenderer>();
				defaultCubeRen->SetOwner(m_DefaultCube.get());
				defaultCubeRen->Init();
				defaultCubeRen->SetMesh(Engine::Framework::MeshLibrary::InstantiateCube());
				defaultCubeRen->GetMaterial()->SetColor(glm::vec4(0.2f, 0.2f, 0.2f, 1)); // Sets ground color to dark grey
				AddRenderable(defaultCubeRen);

				auto defaultCubePhysics = m_DefaultCube->GetComponent<Engine::Framework::Physics::PhysicsComponent>();
				defaultCubePhysics->SetOwner(m_DefaultCube.get());
				defaultCubePhysics->SetMass(1.0f);
				AddPhysicsComponent(defaultCubePhysics);

				AddEntity(std::move(m_DefaultCube));
			}
		}

		Engine::Framework::Physics::PhysicsComponent* GetCubePhysics() {
			if (m_DefaultCube) {
				return m_DefaultCube->GetComponent<Engine::Framework::Physics::PhysicsComponent>();
			}
			return nullptr;
		}
	private:
		std::unique_ptr<Engine::Framework::GameObject> m_GroundCube;
		std::unique_ptr<Engine::Framework::GameObject> m_DefaultCube;
	};
}