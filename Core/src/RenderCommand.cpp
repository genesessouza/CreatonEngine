#include "Engine/Core/RenderCommand.h"

#include <Engine/Rendering/RendererAPI.h>

#include <memory>

namespace Engine::Core
{
	static std::unique_ptr<Engine::Rendering::RendererAPI> s_RendererAPI = Engine::Rendering::RendererAPI::Create();

	void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		s_RendererAPI->SetViewport(x, y, width, height);
	}
}