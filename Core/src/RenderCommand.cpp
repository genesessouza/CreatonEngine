#include "Engine/Core/RenderCommand.h"

#include <Engine/Rendering/RendererAPI.h>
#include <Engine/Rendering/GUIRenderer.h>
#include <Engine/Rendering/Renderer.h>

#include <memory>

namespace Engine::Core
{
	static std::unique_ptr<Engine::Rendering::RendererAPI> s_RendererAPI = Engine::Rendering::RendererAPI::Create();
	static std::unique_ptr<Engine::Rendering::GUIRenderer> s_GUIRenderer = Engine::Rendering::GUIRenderer::Create();

	void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		s_RendererAPI->SetViewport(x, y, width, height);
	}

	void RenderCommand::Clear()
	{
		s_RendererAPI->Clear();
	}

	void RenderCommand::MarkFramebufferDirty()
	{
		Engine::Rendering::Renderer::MarkCameraDirty();
	}

	void RenderCommand::ClearUI()
	{
		s_GUIRenderer->ClearUI();
	}

	void RenderCommand::BeginGUI()
	{
		s_GUIRenderer->Begin();
	}

	void RenderCommand::EndGUI()
	{
		s_GUIRenderer->End();
	}
}