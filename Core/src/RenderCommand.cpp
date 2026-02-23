#include "Engine/Core/RenderCommand.h"

namespace Engine::Core
{
    RendererAPI* RenderCommand::s_API = nullptr;

    void RenderCommand::Init(RendererAPI* api)
    {
        s_API = api;
    }

    void RenderCommand::Clear()
    {
        s_API->Clear();
    }

    void RenderCommand::SetClearColor(float r, float g, float b, float a)
    {
        s_API->SetClearColor(r, g, b, a);
    }

    void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        s_API->SetViewport(x, y, width, height);
    }
}