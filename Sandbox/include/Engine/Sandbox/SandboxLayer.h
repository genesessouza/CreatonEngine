#pragma once

#include <Engine/Core/Layer/Layer.h>
#include <Engine/Core/Event/KeyEvent.h>

#include <Engine/Core/RenderCommand.h>

#include <Engine/Framework/Scene.h>

#include "Engine/Sandbox/MainScene.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine::Sandbox
{
	class SandboxLayer : public Engine::Core::Layer::Layer
	{
	public:
		SandboxLayer() : Layer("Sandbox"), m_CameraMovementDirection(0.0f), m_CameraRotationDirection(0.0f)
		{
		}

		virtual ~SandboxLayer()
		{
			CRTN_LOG_INFO("[SANDBOX LAYER]: Destroyed sandbox layer");
		}

		void OnAttach() override
		{
			Engine::Rendering::Renderer::SetClearColor(glm::vec4(0.529f, 0.808f, 0.922f, 1.0f));
			CRTN_LOG_INFO("[SANDBOX LAYER]: Background color set to: <Sky Blue>\n");

			m_MainScene.Init();
		}

		void OnUpdate(float deltaTime) override
		{
			Engine::Rendering::Renderer::SetClearColor({ 0.529f, 0.808f, 0.922f, 1.0f });

			//CRTN_LOG_INFO("SandboxLayer update");

			m_MainScene.m_SceneCamera->MoveCamera(m_CameraMovementDirection * m_CameraMovementSpeed * deltaTime);
			m_MainScene.m_SceneCamera->RotateCamera(m_CameraRotationDirection * m_CameraRotationSpeed * deltaTime);

			m_MainScene.Render();

			if (m_GroundSpecular < 0) 
				m_GroundSpecular = 0;
			if (m_GroundSpecular > 1)
				m_GroundSpecular = 1;

			if (m_GroundShininess < 8)
				m_GroundShininess = 8;
			if (m_GroundShininess > 128)
				m_GroundShininess = 128;

			if (m_CubeSpecular < 0)
				m_CubeSpecular = 0;
			if (m_CubeSpecular > 1)
				m_CubeSpecular = 1;

			if (m_CubeShininess < 8)
				m_CubeShininess = 8;
			if (m_CubeShininess > 128)
				m_CubeShininess = 128;

			//auto& cameraPos = m_MainScene.m_SceneCamera->GetPosition();
			//CRTN_LOG_DEBUG("Camera Pos: X [%.2f] | Y [%.2f] | Z [%.2f]", cameraPos.x, cameraPos.y, cameraPos.z);
		}

		void OnEditorUpdate(float deltaTime) override
		{

		}

		void OnGUIUpdate() override
		{
			// LOGIC FOR WHEN GAME MENUS ARE IMPLEMENTED GOES HERE
		}

		void OnEvent(Engine::Core::Event::Event& e) override
		{
			//CRTN_LOG_INFO("SandboxLayer registered event: <%s>", e.ToString().c_str());

			Engine::Core::Event::EventDispatcher dispatcher(e);

			dispatcher.Dispatch<Engine::Core::Event::FramebufferResizeEvent>([this](Engine::Core::Event::FramebufferResizeEvent& e)
				{
					m_MainScene.OnViewportResize(e.GetWidth(), e.GetHeight());
					return false;
				});

			// KEY PRESSED
			dispatcher.Dispatch<Engine::Core::Event::KeyPressedEvent>([this](Engine::Core::Event::KeyPressedEvent& e)
				{
					float speed = 1.0f;

					// MOVEMENT
					if (e.GetKeyCode() == GLFW_KEY_W) m_CameraMovementDirection.z = speed;
					if (e.GetKeyCode() == GLFW_KEY_S) m_CameraMovementDirection.z = -speed;

					if (e.GetKeyCode() == GLFW_KEY_A) m_CameraMovementDirection.x = -speed;
					if (e.GetKeyCode() == GLFW_KEY_D) m_CameraMovementDirection.x = speed;

					if (e.GetKeyCode() == GLFW_KEY_SPACE) m_CameraMovementDirection.y = speed;
					if (e.GetKeyCode() == GLFW_KEY_LEFT_SHIFT) m_CameraMovementDirection.y = -speed;

					// ROTATION
					if (e.GetKeyCode() == GLFW_KEY_UP) m_CameraRotationDirection.x = -speed;
					if (e.GetKeyCode() == GLFW_KEY_DOWN) m_CameraRotationDirection.x = speed;

					if (e.GetKeyCode() == GLFW_KEY_LEFT) m_CameraRotationDirection.y = -speed;
					if (e.GetKeyCode() == GLFW_KEY_RIGHT) m_CameraRotationDirection.y = speed;

					if (e.GetKeyCode() == GLFW_KEY_Q) m_CameraRotationDirection.z = -speed;
					if (e.GetKeyCode() == GLFW_KEY_E) m_CameraRotationDirection.z = speed;

					// GROUND
					{
						if (e.GetKeyCode() == GLFW_KEY_1)
						{
							m_GroundShininess *= 2;
							m_MainScene.GetObjects()[1]->m_MeshRenderer->GetMaterial()->SetShininess(m_GroundShininess);

							CRTN_LOG_TRACE("Ground Shininess set to: %f", m_GroundShininess);
						}

						if (e.GetKeyCode() == GLFW_KEY_2)
						{
							m_GroundSpecular += 0.1f;
							m_MainScene.GetObjects()[1]->m_MeshRenderer->GetMaterial()->SetSpecular(m_GroundSpecular);

							CRTN_LOG_TRACE("Ground Specular set to: %f", m_GroundSpecular);
						}

						if (e.GetKeyCode() == GLFW_KEY_9)
						{
							m_GroundShininess /= 2;
							m_MainScene.GetObjects()[1]->m_MeshRenderer->GetMaterial()->SetShininess(m_GroundShininess);

							CRTN_LOG_TRACE("Ground Shininess set to: %f", m_GroundShininess);
						}

						if (e.GetKeyCode() == GLFW_KEY_0)
						{
							m_GroundSpecular -= 0.1f;
							m_MainScene.GetObjects()[1]->m_MeshRenderer->GetMaterial()->SetSpecular(m_GroundSpecular);

							CRTN_LOG_TRACE("Ground Specular set to: %f", m_GroundSpecular);
						}
					}

					// DEFAULT CUBE
					{
						if (e.GetKeyCode() == GLFW_KEY_3)
						{
							m_CubeShininess *= 2;
							m_MainScene.GetObjects()[2]->m_MeshRenderer->GetMaterial()->SetShininess(m_CubeShininess * 2);

							CRTN_LOG_TRACE("Cube Shininess set to: %f", m_CubeShininess);
						}

						if (e.GetKeyCode() == GLFW_KEY_4)
						{
							m_CubeSpecular += 0.1f;
							m_MainScene.GetObjects()[2]->m_MeshRenderer->GetMaterial()->SetSpecular(m_CubeSpecular);

							CRTN_LOG_TRACE("Cube Specular set to: %f", m_CubeSpecular);
						}

						if (e.GetKeyCode() == GLFW_KEY_7)
						{
							m_CubeShininess / 2;
							m_MainScene.GetObjects()[2]->m_MeshRenderer->GetMaterial()->SetShininess(m_CubeShininess);

							CRTN_LOG_TRACE("Cube Shininess set to: %f", m_CubeShininess);
						}

						if (e.GetKeyCode() == GLFW_KEY_8)
						{
							m_CubeSpecular - 0.1f;
							m_MainScene.GetObjects()[2]->m_MeshRenderer->GetMaterial()->SetSpecular(m_CubeSpecular);

							CRTN_LOG_TRACE("Cube Specular set to: %f", m_CubeSpecular);
						}
					}
					return false;
				});

			// KEY RELEASED
			dispatcher.Dispatch<Engine::Core::Event::KeyReleasedEvent>([this](Engine::Core::Event::KeyReleasedEvent& e)
				{
					// MOVEMENT
					if (e.GetKeyCode() == GLFW_KEY_W || e.GetKeyCode() == GLFW_KEY_S) m_CameraMovementDirection.z = 0.0f;
					if (e.GetKeyCode() == GLFW_KEY_A || e.GetKeyCode() == GLFW_KEY_D) m_CameraMovementDirection.x = 0.0f;
					if (e.GetKeyCode() == GLFW_KEY_SPACE || e.GetKeyCode() == GLFW_KEY_LEFT_SHIFT) m_CameraMovementDirection.y = 0.0f;

					// ROTATION
					if (e.GetKeyCode() == GLFW_KEY_UP || e.GetKeyCode() == GLFW_KEY_DOWN) m_CameraRotationDirection.y = 0.0f;
					if (e.GetKeyCode() == GLFW_KEY_LEFT || e.GetKeyCode() == GLFW_KEY_RIGHT) m_CameraRotationDirection.x = 0.0f;
					if (e.GetKeyCode() == GLFW_KEY_Q || e.GetKeyCode() == GLFW_KEY_E) m_CameraRotationDirection.z = 0.0f;

					return false;
				});
		}
	private:
		const float m_CameraMovementSpeed = 10.0f;
		const float m_CameraRotationSpeed = 90.0f;

		glm::vec3 m_CameraMovementDirection;
		glm::vec3 m_CameraRotationDirection;
	private:
		Engine::Sandbox::MainScene m_MainScene;
	private:
		// TESTS
		float m_GroundShininess = 4.0f;
		float m_CubeShininess = 4.0f;

		float m_GroundSpecular = 0.2f;
		float m_CubeSpecular = 0.2f;
	};
}