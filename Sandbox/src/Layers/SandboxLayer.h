#include <Engine/Framework/Scene.h>
#include <Engine/Framework/GameObject.h>
#include <Engine/Framework/MeshLibrary.h>

#include <Engine/Core/Event/KeyEvent.h>
#include <Engine/Core/Event/WindowEvent.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class SandboxLayer
	: public Engine::Core::Layer::Layer
{
public:
	SandboxLayer()
		: Layer("Sandbox"), m_CameraMovementDirection(0.0f), m_CameraRotationDirection(0.0f)
	{
		// CAMERA
		m_MainScene.m_SceneCamera.MoveCamera({ 0, 2, -5 });

		// GROUND CUBE
		auto groundCube = std::make_unique<Engine::Framework::GameObject>();

		groundCube->m_Transform.m_Position = { 0, 0, 0 };
		groundCube->m_Transform.m_Scale = { 10, 0.3, 10 };

		groundCube->m_Renderer = std::make_unique<Engine::Rendering::MeshRenderer>();
		groundCube->m_Renderer->m_Mesh = Engine::Framework::MeshLibrary::InstantiateCube();
		groundCube->m_Renderer->m_Shader = Engine::Rendering::Shader::CreateDefaultShader();
		groundCube->m_Renderer->m_Shader->DefineUniformVec4("u_Color", glm::vec4(0.3, 0.3, 0.3, 1));

		m_MainScene.m_SceneObjects.push_back(std::move(groundCube));

		// DEFAULT CUBE
		auto cube = std::make_unique<Engine::Framework::GameObject>();

		cube->m_Transform.m_Position = { 0, 1, 0 };
		cube->m_Transform.m_Scale = { 1, 1, 1 };

		cube->m_Renderer = std::make_unique<Engine::Rendering::MeshRenderer>();
		cube->m_Renderer->m_Mesh = Engine::Framework::MeshLibrary::InstantiateCube();
		cube->m_Renderer->m_Shader = Engine::Rendering::Shader::CreateDefaultShader();
		cube->m_Renderer->m_Shader->DefineUniformVec4("u_Color", glm::vec4(1, 0, 0, 1)); // Sets cube default color to red

		m_MainScene.m_SceneObjects.push_back(std::move(cube));
	}

	void OnAttach() override
	{
		//CRTN_LOG_INFO("SandboxLayer attached");
	}

	void OnUpdate(float deltaTime) override
	{
		//CRTN_LOG_INFO("SandboxLayer update");

		m_MainScene.m_SceneCamera.MoveCamera(m_CameraMovementDirection * m_CameraMovementSpeed * deltaTime);
		m_MainScene.m_SceneCamera.RotateCamera(m_CameraRotationDirection * m_CameraRotationSpeed * deltaTime);

		m_MainScene.Render();

		auto& cameraPos = m_MainScene.m_SceneCamera.GetPosition();

		//CRTN_LOG_DEBUG("Camera Pos: X [%.2f] | Y [%.2f] | Z [%.2f]", cameraPos.x, cameraPos.y, cameraPos.z);
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
				if (e.GetKeyCode() == GLFW_KEY_LEFT) m_CameraRotationDirection.y = -speed;
				if (e.GetKeyCode() == GLFW_KEY_RIGHT) m_CameraRotationDirection.y = speed;

				if (e.GetKeyCode() == GLFW_KEY_UP) m_CameraRotationDirection.x = -speed;
				if (e.GetKeyCode() == GLFW_KEY_DOWN) m_CameraRotationDirection.x = speed;

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
				//if (e.GetKeyCode() == GLFW_KEY_UP || e.GetKeyCode() == GLFW_KEY_DOWN) m_CameraRotationDirection.y = 0.0f;
				//if (e.GetKeyCode() == GLFW_KEY_LEFT || e.GetKeyCode() == GLFW_KEY_RIGHT) m_CameraRotationDirection.x = 0.0f;

				return false;
			});
	}
private:
	const float m_CameraMovementSpeed = 10.0f;
	const float m_CameraRotationSpeed = 90.0f;

	glm::vec3 m_CameraMovementDirection;
	glm::vec3 m_CameraRotationDirection;
private:
	Engine::Framework::Scene m_MainScene;
};