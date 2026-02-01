#pragma once

#include <Engine/Core/Layer/Layer.h>

#include <Engine/Core/Event/KeyEvent.h>

#include "Engine/Sandbox/MainScene.h"

#include <Engine/Core/RenderCommand.h>

#include <Engine/Framework/Scene.h>
#include <Engine/Framework/Raycast.h>
#include <Engine/Framework/Debugging.h>
#include <Engine/Framework/Camera.h>

#include <Engine/Editor/GUI/GUIUtils.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ImGui/ImGuizmo.h>

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
			//CRTN_LOG_INFO("[SANDBOX LAYER]: Destroyed sandbox layer");
		}

		void OnAttach() override
		{
			Engine::Rendering::Renderer::SetClearColor(glm::vec4(0.529f, 0.808f, 0.922f, 1.0f));
			CRTN_LOG_INFO("[SANDBOX LAYER]: Background color set to: <Sky Blue>\n");

			m_MainScene = std::make_unique<MainScene>();
			CRTN_CHECK_PTR(m_MainScene);
			m_MainScene->Init();
		}

		void OnUpdate(float deltaTime) override
		{
			Engine::Rendering::Renderer::SetClearColor({ 0.529f, 0.808f, 0.922f, 1.0f });

			//CRTN_LOG_INFO("SandboxLayer update");

			glm::vec3 deltaPos = m_CameraMovementDirection * deltaTime;

			m_MainScene->GetSceneCamera()->GetOwner()->GetTransform().Translate(deltaPos);

			m_MainScene->OnUpdateRuntime(deltaTime);
			m_MainScene->OnRender();

			Engine::Framework::Debugging::Render(deltaTime, m_MainScene->GetSceneCamera()->GetViewMatrix(), m_MainScene->GetSceneCamera()->GetProjectionMatrix());
		}

		void OnGUIUpdate() override
		{
		}

		void OnEvent(Engine::Core::Event::Event& e) override
		{
			//CRTN_LOG_INFO("SandboxLayer registered event: <%s>", e.ToString().c_str());

			Engine::Core::Event::EventDispatcher dispatcher(e);

			dispatcher.Dispatch<Engine::Core::Event::FramebufferResizeEvent>([this](Engine::Core::Event::FramebufferResizeEvent& e)
				{
					m_MainScene->OnViewportResize(e.GetWidth(), e.GetHeight());
					return false;
				});

			dispatcher.Dispatch<Engine::Core::Event::MouseClickedEvent>([this](Engine::Core::Event::MouseClickedEvent& e)
				{
					if (e.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
					{
						if (Engine::Editor::GUI::GUIUtils::IsMouseInsideViewport())
						{
							if (ImGuizmo::IsOver())
								return true;

							auto& app = Engine::Core::Application::Get();
							m_LastMousePos = glm::vec2(app.GetMousePosition().XPos, app.GetMousePosition().YPos);
							m_IsDragging = true;

							Engine::Framework::Raycast::RayResult result = Engine::Framework::Raycast::MouseToWorldPos(*m_MainScene->GetSceneCamera(), false);
							if (result.Success)
							{
								m_SelectedEntity = result.HitEntity;
								Engine::Editor::EditorGUI::Get().SelectEntity(m_SelectedEntity);
							}
							else
							{
								m_SelectedEntity = nullptr;
								Engine::Editor::EditorGUI::Get().SelectEntity(m_SelectedEntity);
							}
						}
						return true;
					}
				});

			dispatcher.Dispatch<Engine::Core::Event::MouseReleasedEvent>([this](Engine::Core::Event::MouseReleasedEvent& e)
				{
					if (e.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
						m_IsDragging = false;

					return true;
				});

			dispatcher.Dispatch<Engine::Core::Event::MouseMovedEvent>([this](Engine::Core::Event::MouseMovedEvent& e)
				{
					if (ImGuizmo::IsUsing() || !m_IsDragging)
						return false;

					glm::vec2 currentMousePos = { e.GetX(), e.GetY() };
					glm::vec2 delta = currentMousePos - m_LastMousePos;
					m_LastMousePos = currentMousePos;

					float yaw = -delta.x * m_Sensitivity;
					float pitch = -delta.y * m_Sensitivity;

					glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));

					glm::quat cameraOrientation = m_MainScene->GetSceneCamera()->GetOwner()->GetTransform().GetOrientation();
					glm::vec3 right = cameraOrientation * glm::vec3(1, 0, 0);
					glm::quat qPitch = glm::angleAxis(pitch, right);

					m_MainScene->GetSceneCamera()->GetOwner()->GetTransform().SetRotation(glm::degrees(glm::eulerAngles(qPitch * qYaw * cameraOrientation)));
					cameraOrientation = glm::normalize(cameraOrientation);

					return true;
				});

			// KEY PRESSED
			dispatcher.Dispatch<Engine::Core::Event::KeyPressedEvent>([this](Engine::Core::Event::KeyPressedEvent& e)
				{
					float speed = 1.0f;

					if (e.GetKeyCode() == GLFW_KEY_SPACE)
					{
						auto cubePhysics = m_MainScene->GetPhysicsComponents()[1];
						cubePhysics->SetAngularVelocity(glm::vec3(0, 3, 0));
					}

					// MOVEMENT
					if (e.GetKeyCode() == GLFW_KEY_W) m_CameraMovementDirection.z = -speed * m_CameraMovementSpeed;
					if (e.GetKeyCode() == GLFW_KEY_S) m_CameraMovementDirection.z = speed * m_CameraMovementSpeed;

					if (e.GetKeyCode() == GLFW_KEY_A) m_CameraMovementDirection.x = -speed * m_CameraMovementSpeed;
					if (e.GetKeyCode() == GLFW_KEY_D) m_CameraMovementDirection.x = speed * m_CameraMovementSpeed;

					if (e.GetKeyCode() == GLFW_KEY_SPACE) m_CameraMovementDirection.y = speed * m_CameraMovementSpeed;
					if (e.GetKeyCode() == GLFW_KEY_LEFT_SHIFT) m_CameraMovementDirection.y = -speed * m_CameraMovementSpeed;

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
		std::unique_ptr<Engine::Sandbox::MainScene> m_MainScene;
		Engine::Framework::Entity* m_SelectedEntity = nullptr;
	private:
		// For camera rotation
		float m_CameraMovementSpeed = 10.0f;
		float m_CameraRotationSpeed = 90.0f;

		glm::vec3 m_CameraMovementDirection;
		glm::vec3 m_CameraRotationDirection;

		bool m_IsDragging = false;
		glm::vec2 m_LastMousePos = { 0.0f, 0.0f };
		float m_Sensitivity = 0.005f;
	};
}