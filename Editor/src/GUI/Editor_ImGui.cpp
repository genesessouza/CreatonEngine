#include "Engine/Editor/GUI/EditorGUI.h"

#include <Engine/Core/Application.h>

#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

namespace Engine::Editor
{
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

	void EditorGUI::OnGUIRender()
	{
		// DOCKING INITIALIZATION
		{
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
		}

		// NAVBAR MENU
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
						/* EXAMPLE CODE FOR SCENE CREATION
						


						*/
					}
					if (ImGui::MenuItem("Game Object"))
					{

					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
		}

		// SCENE OBJECTS PANEL
		{
			ImGui::Begin("Scene Objects");
			ImGui::Text("Entities:");
			if (ImGui::TreeNode("Main Camera")) { ImGui::TreePop(); }
			if (ImGui::TreeNode("Directional Light")) { ImGui::TreePop(); }
			if (ImGui::TreeNode("Player (Cube)")) { ImGui::TreePop(); }
			ImGui::End();
		}

		// SCENE VIEWPORT PANEL
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin("Scene");
			ImGui::PopStyleVar();

			float size = ImGui::GetFontSize() * 5.0f;
			ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

			if (m_SceneState == Engine::Core::SceneState::State::Edit) {
				if (ImGui::Button("Play", ImVec2(size, 0))) 
				{
					m_SceneState = Engine::Core::SceneState::State::Play;
				}
			}
			else {
				if (ImGui::Button("Stop", ImVec2(size, 0))) 
				{
					m_SceneState = Engine::Core::SceneState::State::Edit;
				}
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

			ImGui::End();
		}

		// CONSOLE PANEL
		{
			ImGui::Begin("Editor Console");
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "[WARNING]: Shader 'Lit' compilation warning...");
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "[INFO]: Scene 'Level_01' loaded successfully.");
			if (ImGui::Button("Clear")) { /* Limpar Logs */ }
			ImGui::End();
		}

		// OBJECT INFO PANEL
		{
			ImGui::Begin("Object Info");
			ImGui::Text("Transform");
			static float pos[3] = { 0, 0, 0 };
			ImGui::DragFloat3("Position", pos, 0.1f);

			static float color[4] = { 1, 1, 1, 1 };
			ImGui::ColorEdit4("Albedo", color);
			ImGui::End();
		}

		ImGui::End(); // End DockSpace
	}

	void EditorGUI::OnShutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}