#include "Engine/Editor/GUI/EditorGUI.h"

#include <Engine/Core/Application.h>

#include <Engine/Framework/Camera.h>
#include <Engine/Framework/Scene.h>
#include <Engine/Framework/GameObject.h>
#include <Engine/Framework/Light.h>
#include <Engine/Framework/MeshLibrary.h>

#include <Engine/Framework/Physics/Collider.h>
#include <Engine/Framework/Physics/PhysicsComponent.h>

#include <Engine/Rendering/MeshRenderer.h>

#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include <glm/gtc/type_ptr.hpp>

namespace Engine::Editor
{
	static EditorGUI* s_EditorGUIInstance = nullptr;

	EditorGUI::EditorGUI()
	{
		s_EditorGUIInstance = this;
	}

	EditorGUI& EditorGUI::Get()
	{
		return *s_EditorGUIInstance;
	}

	void EditorGUI::OnInit()
	{
		ImGui::CreateContext();

		// DOCKING AND VIEWPORTS PARAMS
		{
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

			ImGui::StyleColorsDark();

			ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)(GLFWwindow*)Engine::Core::Application::Get().GetNativeWindow(), true);
			ImGui_ImplOpenGL3_Init("#version 450");
		}

		m_ImGuiActive = true;
	}

	void EditorGUI::DrawNavbar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open")) {/* IMPORT SOMETHING HERE */ }
				if (ImGui::MenuItem("Exit")) Engine::Core::Application::Get().Close();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("New"))
			{
				if (ImGui::MenuItem("Scene"))
				{
					//auto newScene = std::make_shared<Engine::Framework::Scene>();
					//newScene->Init();

					//auto& currentScene = Engine::Framework::Scene::Get();
					//currentScene.SetActiveScene(newScene.get());
				}

				if (ImGui::BeginMenu("Light"))
				{
					if (ImGui::MenuItem("Point Light"))
					{
						//auto purpleLight = Engine::Framework::Lights::PointLight::Cre("[Point Light] New Light");
						//purpleLight.GetTransform().SetPosition({ 0.0f, 1.0f, -3.0f });
						//purpleLight.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
						//purpleLight.SetIntensity(10.0f);
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Primitives"))
				{
					if (ImGui::MenuItem("3D Cube"))
					{
						auto newCube = Engine::Framework::GameObject::Create("[GameObject] New Cube");

						newCube->AddComponent<Engine::Rendering::MeshRenderer>();
						newCube->GetComponent<Engine::Rendering::MeshRenderer>()->SetMesh(Engine::Framework::MeshLibrary::InstantiateCube());
						newCube->GetComponent<Engine::Rendering::MeshRenderer>()->GetMaterial()->SetColor(glm::vec4(0.2f, 0.2f, 0.2f, 1));

						newCube->AddComponent<Engine::Framework::Physics::CubeCollider>(glm::vec3(1.0f));
						newCube->AddComponent<Engine::Framework::Physics::PhysicsComponent>();

						newCube->GetTransform().SetPosition({ 0.0f, 1.0f, 0.0f });
						newCube->GetTransform().SetScale({ 1.0f, 1.0f, 1.0f });
						
						//newCube->Init();
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}

	void EditorGUI::DrawHierarchy()
	{
		ImGui::Begin("Hierarchy");
		auto& currentScene = Engine::Framework::Scene::Get();

		auto& entities = currentScene.GetEntities();
		for (size_t i = 0; i < entities.size(); i++)
		{
			auto& entity = entities[i];

			ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity.get()) ? ImGuiTreeNodeFlags_Selected : 0);
			flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;

			bool opened = ImGui::TreeNodeEx((void*)(uintptr_t)i, flags, entity->GetName().c_str());

			if (ImGui::IsItemClicked())
				m_SelectedEntity = entity.get();

			if (opened) ImGui::TreePop();
		}

		ImGui::End();
	}

	void EditorGUI::DrawSceneViewport()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Scene");
		ImGui::PopStyleVar();

		float size = ImGui::GetFontSize() * 5.0f;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

		if (m_SceneState == Engine::Core::SceneState::State::Edit)
		{
			if (ImGui::Button("Play", ImVec2(size, 0)))
				m_SceneState = Engine::Core::SceneState::State::Play;
		}
		else
		{
			if (ImGui::Button("Stop", ImVec2(size, 0)))
				m_SceneState = Engine::Core::SceneState::State::Edit;
		}

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		if (viewportPanelSize.x > 0.0f && viewportPanelSize.y > 0.0f)
		{
			if (m_ViewportSize.x != viewportPanelSize.x || m_ViewportSize.y != viewportPanelSize.y)
			{
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

				if (m_ResizeCallback)
					m_ResizeCallback((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			}
		}

		uint32_t textureID = Engine::Core::Application::Get().GetFramebuffer()->GetColorAttachmentRendererID();
		ImGui::Image((void*)(uintptr_t)textureID, { m_ViewportSize.x, m_ViewportSize.y }, { 0, 1 }, { 1, 0 });

		// MOUSE RAYCASTING ON SCENE VIEWPORT FOR PICKING
		{
			ImVec2 vMin = ImGui::GetItemRectMin();
			ImVec2 vMax = ImGui::GetItemRectMax();

			m_ViewportBounds[0] = { vMin.x, vMin.y };
			m_ViewportBounds[1] = { vMax.x, vMax.y };
		}

		ImGui::End();
	}

	void EditorGUI::DrawConsole()
	{
		ImGui::Begin("Editor Console");
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "[WARNING]: Shader 'Lit' compilation warning...");
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "[INFO]: Scene 'Level_01' loaded successfully.");
		if (ImGui::Button("Clear")) { /* Clear Logs */ }
		ImGui::End();
	}

	void EditorGUI::DrawObjectInfo()
	{
		ImGui::Begin("Object Info");

		if (!m_SelectedEntity)
		{
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "No entity selected.");
			ImGui::End();
			return;
		}

		bool enabled = m_SelectedEntity->IsEnabled();
		if (ImGui::Checkbox("##Enabled", &enabled))
			m_SelectedEntity->SetEnabled(enabled);

		ImGui::SameLine();

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strncpy_s(buffer, m_SelectedEntity->GetName().c_str(), sizeof(buffer));

		if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
			m_SelectedEntity->SetName(std::string(buffer));

		ImGui::Separator();

		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			auto& transform = m_SelectedEntity->GetTransform();

			glm::vec3 pos = transform.GetPosition();
			glm::vec3 rot = transform.GetRotation();
			glm::vec3 scl = transform.GetScale();

			if (ImGui::DragFloat3("Position", glm::value_ptr(pos), 0.1f))
				transform.SetPosition(pos);

			if (ImGui::DragFloat3("Rotation", glm::value_ptr(rot), 0.1f))
				transform.SetRotation(rot);

			if (ImGui::DragFloat3("Scale", glm::value_ptr(scl), 0.1f, 0.001f))
				transform.SetScale(scl);
		}

		if (auto light = dynamic_cast<Engine::Framework::Lights::PointLight*>(m_SelectedEntity))
			DrawPointLightUI(light);
		else if (auto dirLight = dynamic_cast<Engine::Framework::Lights::DirectionalLight*>(m_SelectedEntity))
			DrawDirectionalLightUI(dirLight);
		else if (auto gameObject = dynamic_cast<Engine::Framework::GameObject*>(m_SelectedEntity))
			DrawGameObjectUI(gameObject);
		else if (auto camera = dynamic_cast<Engine::Framework::Camera*>(m_SelectedEntity))
			DrawCameraUI(camera);

		ImGui::End();
	}

	void EditorGUI::OnGUIRender()
	{
		// DOCKING INITIALIZATION
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace", nullptr, window_flags);
		ImGui::PopStyleVar(3);

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		DrawNavbar();
		DrawHierarchy();
		DrawSceneViewport();
		DrawConsole();
		DrawObjectInfo();

		ImGui::End(); // End DockSpace
	}

	void EditorGUI::DrawPointLightUI(Engine::Framework::Lights::PointLight* light)
	{
		if (ImGui::CollapsingHeader("Point Light", ImGuiTreeNodeFlags_DefaultOpen))
		{
			glm::vec4 color = light->GetColor();
			float intensity = light->GetIntensity();

			if (ImGui::ColorEdit4("Color", glm::value_ptr(color)))
				light->SetColor(color);

			if (ImGui::DragFloat("Intensity", &intensity, 0.5f, 0.0f, 100.0f))
				light->SetIntensity(intensity);
		}
	}

	void EditorGUI::DrawDirectionalLightUI(Engine::Framework::Lights::DirectionalLight* light)
	{
		if (ImGui::CollapsingHeader("Directional Light", ImGuiTreeNodeFlags_DefaultOpen))
		{
			glm::vec3 dir = light->GetOwner()->GetTransform().GetRotation();
			float intensity = light->GetIntensity();

			if (ImGui::DragFloat3("Direction", glm::value_ptr(dir), 0.05f))
				light->GetOwner()->GetTransform().SetRotation(dir);

			if (ImGui::SliderFloat("Intensity", &intensity, 0.5f, 30.0f))
				light->SetIntensity(intensity);
		}
	}

	void EditorGUI::DrawGameObjectUI(Engine::Framework::GameObject* obj)
	{
		auto& meshRenderer = *obj->GetComponent<Engine::Rendering::MeshRenderer>();
		if (!&meshRenderer) return;

		if (ImGui::CollapsingHeader("Object Data", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::TreeNodeEx("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen))
			{
				auto& material = meshRenderer.GetMaterial();
				auto& mesh = meshRenderer.GetMesh();

				if (mesh && ImGui::TreeNodeEx("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Text("[Not yet] Mesh Type: ");
					ImGui::TreePop();
				}
				if (material && ImGui::TreeNodeEx("Material", ImGuiTreeNodeFlags_DefaultOpen))
				{
					glm::vec4 color = material->GetColor();

					float diff = material->GetDiffuse();
					float spec = material->GetSpecular();
					float shin = material->GetShininess();

					if (ImGui::ColorEdit4("Albedo", glm::value_ptr(color)))
						material->SetColor(color);

					if (ImGui::SliderFloat("Diffuse", &diff, 0.0f, 1.0f))
						material->SetDiffuse(diff);

					if (ImGui::SliderFloat("Specular", &spec, 0.0f, 1.0f))
						material->SetSpecular(spec);

					if (ImGui::SliderFloat("Shininess", &shin, 8.0f, 128.0f))
						material->SetShininess(shin);

					ImGui::TreePop();
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNodeEx("Components", ImGuiTreeNodeFlags_DefaultOpen))
			{
				auto phys = obj->GetComponent<Engine::Framework::Physics::PhysicsComponent>();
				if (phys)
				{
					if (ImGui::TreeNodeEx("Physics", ImGuiTreeNodeFlags_DefaultOpen))
					{
						bool isStatic = phys->IsStatic();
						if (ImGui::Checkbox("Static", &isStatic))
							phys->SetStatic(isStatic);

						bool isEnabled = phys->IsEnabled();
						if (ImGui::Checkbox("Enabled", &isEnabled))
							phys->SetEnabled(isEnabled);

						glm::vec3 vel = phys->GetVelocity();
						if (ImGui::DragFloat3("Velocity", &vel.x, 0.1f))
							phys->SetVelocity(vel);

						glm::vec3 angVel = phys->GetAngularVelocity();
						if (ImGui::DragFloat3("Angular Velocity", &angVel.x, 0.1f))
							phys->SetAngularVelocity(angVel);

						float mass = phys->GetMass();
						if (ImGui::DragFloat("Mass", &mass, 0.1f, 0.01f, 1000.0f))
							phys->SetMass(mass);

						ImGui::TreePop();
					}
				}

				auto col = obj->GetComponent<Engine::Framework::Physics::CubeCollider>();
				if (col)
				{
					if (ImGui::TreeNodeEx("Collider", ImGuiTreeNodeFlags_DefaultOpen))
					{
						bool enabled = col->IsEnabled();
						bool isTrigger = col->IsTrigger();

						if (ImGui::Checkbox("Enabled", &enabled))
							col->SetEnabled(enabled);

						if (ImGui::Checkbox("Trigger", &isTrigger))
							col->SetTrigger(isTrigger);

						ImGui::TreePop();
					}
				}

				/*auto dirLight = obj->GetComponent<Engine::Framework::Lights::DirectionalLight>();
				if (ImGui::TreeNodeEx("Directional Light", ImGuiTreeNodeFlags_DefaultOpen))
				{
					bool enabled = dirLight->IsEnabled();

					if (ImGui::Checkbox("Enabled", &enabled))
						col->SetEnabled(enabled);

					glm::vec3 dir = dirLight->GetOwner()->GetTransform().GetRotation();
					float intensity = dirLight->GetIntensity();

					glm::vec4 color = dirLight->GetColor();

					if (ImGui::ColorEdit4("Color", glm::value_ptr(color)))
						dirLight->SetColor(color, intensity);

					if (ImGui::DragFloat3("Direction", glm::value_ptr(dir), 0.05f))
						dirLight->SetDirection(dir);

					if (ImGui::SliderFloat("Intensity", &intensity, 0.5f, 30.0f))
						dirLight->SetIntensity(intensity);

					ImGui::TreePop();
				}*/

				ImGui::TreePop();
			}
		}
	}

	void EditorGUI::DrawCameraUI(Engine::Framework::Camera* cam)
	{
		if (ImGui::CollapsingHeader("Parameters", ImGuiTreeNodeFlags_DefaultOpen))
		{
			auto camFov = cam->GetFOV();
			auto camNear = cam->GetNear();
			auto camFar = cam->GetFar();

			if (ImGui::SliderFloat("Field of View", &camFov, 10.0f, 170.0f))
				cam->SetFOV(camFov);

			if (ImGui::SliderFloat("Near Plane", &camNear, 0.001f, 10.0f))
				cam->SetNear(camNear);

			if (ImGui::SliderFloat("Far Plane", &camFar, 30.0f, 1000.0f))
				cam->SetFar(camFar);
		}
	}

	void EditorGUI::OnShutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}