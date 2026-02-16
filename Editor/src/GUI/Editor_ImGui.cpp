#include "Engine/Editor/GUI/EditorGUI.h"

#include <Engine/Core/Application.h>

#include <Engine/Framework/Camera.h>
#include <Engine/Framework/Scene.h>
#include <Engine/Framework/Entity.h>
#include <Engine/Framework/Light.h>
#include <Engine/Framework/MeshLibrary.h>

#include <Engine/Framework/Physics/Collider.h>
#include <Engine/Framework/Physics/PhysicsComponent.h>

#include <Engine/Rendering/MeshRenderer.h>

#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/ImGuizmo.h>

#include <glm/gtc/type_ptr.hpp>

#define CRT_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

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
			//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

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
					//auto newScene = Engine::Framework::Scene::Create();
					//newScene->Init();

					//auto& currentScene = Engine::Framework::Scene::Get();
					//currentScene.SetActiveScene(newScene.get());
				}

				if (ImGui::BeginMenu("Light"))
				{
					if (ImGui::MenuItem("Point Light"))
					{
						auto newLightGo = Engine::Framework::Entity::CreateBillboard("[Point Light] New Light", 1.0f, glm::vec4(1.0f));
						newLightGo->GetTransform().SetPosition({ 0.0f, 1.0f, -3.0f });

						auto newLight = newLightGo->AddComponent<Engine::Framework::Lights::PointLight>();
						newLight->SetIntensity(10.0f);
						newLight->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

						Engine::Framework::Scene::Get().AddEntity(std::move(newLightGo));
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Primitives"))
				{
					if (ImGui::MenuItem("3D Cube"))
					{
						auto newCube = Engine::Framework::Entity::CreateWithCollider("[Entity] New Cube", Engine::Framework::MeshLibrary::InstantiateCube());
						newCube->GetTransform().SetPosition({ 0.0f, 1.0f, 0.0f });
						newCube->GetTransform().SetScale({ 1.0f, 1.0f, 1.0f });
						newCube->GetComponent<Engine::Rendering::MeshRenderer>()->GetMaterial()->SetColor(glm::vec4(0.2f, 0.2f, 0.2f, 1));

						Engine::Framework::Scene::Get().AddEntity(std::move(newCube));
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

		auto& activeScene = Engine::Framework::Scene::Get();

		if (activeScene.GetSceneState() == Engine::Framework::Scene::SceneState::Edit)
		{
			if (ImGui::Button("Play", ImVec2(size, 0)))
				activeScene.SetSceneState(Engine::Framework::Scene::Play);
		}
		else if (activeScene.GetSceneState() == Engine::Framework::Scene::SceneState::Play)
		{
			if (ImGui::Button("Pause", ImVec2(size, 0)))
				activeScene.SetSceneState(Engine::Framework::Scene::Pause);
		}
		else
		{
			if (ImGui::Button("Edit", ImVec2(size, 0)))
				activeScene.SetSceneState(Engine::Framework::Scene::Edit);
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

		if (m_SelectedEntity)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());

			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportSize.x, m_ViewportSize.y);

			auto sceneCam = Engine::Framework::Scene::Get().GetSceneCamera();

			const glm::mat4& view = sceneCam->GetViewMatrix();
			const glm::mat4& proj = sceneCam->GetProjectionMatrix();

			glm::mat4 modelMatrix = m_SelectedEntity->GetTransform().GetWorldMatrix();

			ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj), ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, glm::value_ptr(modelMatrix));

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 pos{}, rot{}, scl{};
				glm::quat q{};

				glm::vec3 skew;
				glm::vec4 persp;

				auto& t = m_SelectedEntity->GetTransform();
				t.SetPosition(pos);
				t.SetRotation(glm::degrees(rot));
				t.SetScale(scl);

				glm::decompose(modelMatrix, scl, q, pos, skew, persp);
				rot = glm::eulerAngles(q);
			}
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
			{
				glm::vec3 newPos = pos - transform.GetPosition();
				transform.Translate(newPos, false);
				pos = newPos;
			}

			if (ImGui::DragFloat3("Rotation", glm::value_ptr(rot)))
			{
				glm::vec3 newRot = rot - transform.GetRotation();
				transform.Rotate(newRot);
				rot = newRot;
			}

			if (ImGui::DragFloat3("Scale", glm::value_ptr(scl), 0.1f, 0.001f))
				transform.SetScale(scl);
		}

		DrawEntityUI(m_SelectedEntity);

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

	void EditorGUI::DrawEntityUI(Engine::Framework::Entity* obj)
	{
		auto& meshRenderer = *obj->GetComponent<Engine::Rendering::MeshRenderer>();

		if (&meshRenderer)
		{
			if (ImGui::CollapsingHeader("Rendering Data", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::TreeNodeEx("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen))
				{
					auto& mesh = meshRenderer.GetMesh();

					if (mesh && ImGui::TreeNodeEx("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
					{
						auto meshType = mesh->GetMeshCPU().MeshTypeToString();

						ImGui::Text("Mesh Type: %s", meshType);
					}
					ImGui::TreePop();

					auto& material = meshRenderer.GetMaterial();
					if (material && ImGui::TreeNodeEx("Material", ImGuiTreeNodeFlags_DefaultOpen))
					{
						glm::vec4 color = material->GetColor();

						float diff = material->GetDiffuse();
						float spec = material->GetSpecular();
						glm::vec4 specColor = material->GetSpecularColor();
						float shin = material->GetShininess();

						if (ImGui::ColorEdit4("Albedo", glm::value_ptr(color)))
							material->SetColor(color);

						if (ImGui::SliderFloat("Diffuse", &diff, 0.0f, 1.0f))
							material->SetDiffuse(diff);

						if (ImGui::SliderFloat("Specular", &spec, 0.0f, 1.0f))
							material->SetSpecular(spec);

						if (ImGui::ColorEdit4("Specular Color", glm::value_ptr(specColor)))
							material->SetSpecularColor(color);

						if (ImGui::SliderFloat("Shininess", &shin, 8.0f, 128.0f))
							material->SetShininess(shin);

						ImGui::TreePop();
					}
					ImGui::TreePop();
				}
			}
		}

		if (ImGui::CollapsingHeader("Components", ImGuiTreeNodeFlags_DefaultOpen))
		{
			auto phys = obj->GetComponent<Engine::Framework::Physics::PhysicsComponent>();
			if (phys)
			{
				if (ImGui::TreeNodeEx("Physics", ImGuiTreeNodeFlags_DefaultOpen))
				{
					bool isStatic = phys->IsStatic();
					if (ImGui::Checkbox("Static", &isStatic))
						phys->SetStatic(isStatic);

					bool isEnabled = phys->IsEnabled() && m_SelectedEntity->IsEnabled();
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
					bool enabled = col->IsEnabled() && m_SelectedEntity->IsEnabled();
					bool isTrigger = col->IsTrigger();

					if (ImGui::Checkbox("Enabled", &enabled))
						col->SetEnabled(enabled);

					if (ImGui::Checkbox("Trigger", &isTrigger))
						col->SetTrigger(isTrigger);

					ImGui::TreePop();
				}
			}

			auto light = obj->GetComponent<Engine::Framework::Lights::Light>();
			if (light)
			{
				if (ImGui::TreeNodeEx("Light", ImGuiTreeNodeFlags_DefaultOpen))
				{
					bool enabled = light->IsEnabled() && m_SelectedEntity->IsEnabled();

					if (ImGui::Checkbox("Enabled", &enabled))
						light->SetEnabled(enabled);

					glm::vec3 dir = light->GetDirection();
					float intensity = light->GetIntensity();

					glm::vec4 color = light->GetColor();

					if (ImGui::ColorEdit4("Color", glm::value_ptr(color)))
						light->SetColor(color);

					if (ImGui::DragFloat3("Direction", glm::value_ptr(dir), 0.05f))
					{
						light->SetDirection(dir);
						light->GetOwner()->GetTransform().SetRotation(dir);
					}

					if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 30.0f))
						light->SetIntensity(intensity);

					auto billboard = obj->GetComponent<Engine::Framework::Billboard>();
					if (billboard)
					{
						bool enabled = billboard->IsEnabled();

						if (ImGui::TreeNodeEx("Billboard", ImGuiTreeNodeFlags_DefaultOpen))
						{
							if (ImGui::Checkbox("Enabled", &enabled))
								billboard->SetEnabled(enabled);

							glm::vec3 pos = billboard->GetOwner()->GetTransform().GetPosition();
							glm::vec4 color = billboard->GetColor();
							float size = billboard->GetSize();

							if (ImGui::DragFloat3("Position", glm::value_ptr(pos), 0.05f))
								billboard->GetOwner()->GetTransform().SetPosition(pos);

							if (ImGui::ColorEdit4("Color", glm::value_ptr(color)))
								billboard->SetColor(color);

							if (ImGui::SliderFloat("Size", &size, 0.01f, 30.0f))
								billboard->SetSize(size);

							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
			}

			auto cam = obj->GetComponent<Engine::Framework::Camera>();
			if (cam)
			{
				if (ImGui::TreeNodeEx("Camera", ImGuiTreeNodeFlags_DefaultOpen))
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

					auto billboard = obj->GetComponent<Engine::Framework::Billboard>();
					if (billboard)
					{
						bool enabled = billboard->IsEnabled() && m_SelectedEntity->IsEnabled();

						if (ImGui::TreeNodeEx("Billboard", ImGuiTreeNodeFlags_DefaultOpen))
						{
							if (ImGui::Checkbox("Enabled", &enabled))
								billboard->SetEnabled(enabled);

							glm::vec3 pos = billboard->GetOwner()->GetTransform().GetPosition();
							glm::vec4 color = billboard->GetColor();
							float size = billboard->GetSize();

							if (ImGui::DragFloat3("Position", glm::value_ptr(pos), 0.05f))
								billboard->GetOwner()->GetTransform().SetPosition(pos);

							if (ImGui::ColorEdit4("Color", glm::value_ptr(color)))
								billboard->SetColor(color);

							if (ImGui::SliderFloat("Size", &size, 0.01f, 30.0f))
								billboard->SetSize(size);

							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
			}
		}
	}

	void EditorGUI::OnShutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}