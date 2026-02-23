#include "Engine/Platform/ImGui/ImGuiRenderer.h"

#include <Engine/Core/Log/Logger.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine::Platform::GUI
{
	void ImGuiRenderer::ClearUI()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Dark grey
	}

	void ImGuiRenderer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiRenderer::End()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// FOR DOCKSPACE AND VIEWPORTS
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();

			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			glfwMakeContextCurrent(backup_current_context);
		}
	}
}