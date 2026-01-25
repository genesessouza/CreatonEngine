#pragma once

#include <Engine/Rendering/RendererAPI.h>
#include <Engine/Rendering/GUIRenderer.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <cstdint>

namespace Engine::Core
{
	class RenderCommand 
	{
	public:
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void Clear();

		static void MarkFramebufferDirty();
		
		static void ClearUI();
		static void BeginGUI();
		static void EndGUI();

		static Engine::Rendering::RendererAPI* GetRendererAPI() { return s_RendererAPI; }
		static Engine::Rendering::GUIRenderer* GetGUIRenderer() { return s_GUIRenderer; }
	private:
		static Engine::Rendering::RendererAPI* s_RendererAPI;
		static Engine::Rendering::GUIRenderer* s_GUIRenderer;
	};
}