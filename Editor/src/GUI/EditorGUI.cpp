#include "Engine/Editor/GUI/EditorGUI.h"

#include <Engine/Core/Application.h>

#include <ImGui/imgui_impl_glfw.h>

namespace Engine::Editor
{
    EditorGUI::EditorGUI()
    {

    }

	void EditorGUI::Init()
	{
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)Engine::Core::Application::Get().GetWindow().GetNativeWindow(), true);
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_NewFrame();

        ImGui::Begin("My First Tool", &m_ImGuiActive, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
                if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
                if (ImGui::MenuItem("Close", "Ctrl+W")) { m_ImGuiActive = false; }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::Render();
        ImGui::GetDrawData();

        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
	}
}