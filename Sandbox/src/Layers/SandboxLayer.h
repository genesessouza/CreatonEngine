#include <Engine/Framework/Scene.h>
#include <Engine/Framework/GameObject.h>
#include <Engine/Framework/MeshLibrary.h>

#include <Engine/Core/Event/KeyEvent.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Engine/Core/Event/WindowEvent.h>

class SandboxLayer
	: public Engine::Core::Layer::Layer
{
public:
	SandboxLayer()
		: Layer("Sandbox")
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		// CAMERA
		m_MainScene.m_SceneCamera.MoveCamera({ 0, 2, -5 });

		// GROUND PLANE
		auto quad = std::make_unique<Engine::Framework::GameObject>();

		quad->m_Transform.m_Position = { 0, 0, 0 };
		quad->m_Transform.m_Rotation = { 0,0,0 };
		quad->m_Transform.m_Scale = { 10, 1, 10 };

		quad->m_Renderer = std::make_unique<Engine::Rendering::MeshRenderer>();
		quad->m_Renderer->m_Mesh = Engine::Framework::MeshLibrary::InstantiateQuad();
		quad->m_Renderer->m_Shader = Engine::Rendering::Shader::CreateDefaultShader();

		m_MainScene.m_SceneObjects.push_back(std::move(quad));

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
		CRTN_LOG_INFO("SandboxLayer attached");
	}

	void OnUpdate(float deltaTime) override
	{
		//CRTN_LOG_INFO("SandboxLayer update");

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

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

		if (e.GetType() != Engine::Core::Event::EventType::KeyPressed &&
			e.GetType() != Engine::Core::Event::EventType::KeyReleased)
			return;

		auto& keyEvt = static_cast<Engine::Core::Event::KeyEvent&>(e);

		const float speed = 1.0f;

		if (e.GetType() == Engine::Core::Event::EventType::KeyPressed)
		{
			if (keyEvt.GetKeyCode() == GLFW_KEY_W) m_CameraMovementDirection.z = speed;
			if (keyEvt.GetKeyCode() == GLFW_KEY_S) m_CameraMovementDirection.z = -speed;

			if (keyEvt.GetKeyCode() == GLFW_KEY_A) m_CameraMovementDirection.x = -speed;
			if (keyEvt.GetKeyCode() == GLFW_KEY_D) m_CameraMovementDirection.x = speed;

			if (keyEvt.GetKeyCode() == GLFW_KEY_SPACE) m_CameraMovementDirection.y = speed;
			if (keyEvt.GetKeyCode() == GLFW_KEY_LEFT_SHIFT) m_CameraMovementDirection.y = -speed;

			if (keyEvt.GetKeyCode() == GLFW_KEY_LEFT) m_CameraRotationDirection.y = -speed;
			if (keyEvt.GetKeyCode() == GLFW_KEY_RIGHT) m_CameraRotationDirection.y = speed;

			if (keyEvt.GetKeyCode() == GLFW_KEY_UP) m_CameraRotationDirection.x = -speed;
			if (keyEvt.GetKeyCode() == GLFW_KEY_DOWN) m_CameraRotationDirection.x = speed;
		}

		if (e.GetType() == Engine::Core::Event::EventType::KeyReleased)
		{
			if (keyEvt.GetKeyCode() == GLFW_KEY_W || keyEvt.GetKeyCode() == GLFW_KEY_S)
				m_CameraMovementDirection.z = 0.0f;

			if (keyEvt.GetKeyCode() == GLFW_KEY_A || keyEvt.GetKeyCode() == GLFW_KEY_D)
				m_CameraMovementDirection.x = 0.0f;

			if (keyEvt.GetKeyCode() == GLFW_KEY_SPACE || keyEvt.GetKeyCode() == GLFW_KEY_LEFT_SHIFT)
				m_CameraMovementDirection.y = 0.0f;

			if (keyEvt.GetKeyCode() == GLFW_KEY_Q || keyEvt.GetKeyCode() == GLFW_KEY_E)
				m_CameraRotationDirection.y = 0.0f;
		}
	}
private:
	const float m_CameraMovementSpeed = 10.0f;
	const float m_CameraRotationSpeed = 90.0f;

	glm::vec3 m_CameraMovementDirection;
	glm::vec3 m_CameraRotationDirection;
private:
	Engine::Framework::Scene m_MainScene;
};