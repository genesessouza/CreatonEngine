#include "Engine/Rendering/RendererAPI.h"

#include <Engine/Platform/OpenGL/OpenGLRendererAPI.h>

#include <Engine/Core/Log/Logger.h>

namespace Engine::Rendering
{
    RendererAPI::API RendererAPI::GetAPI()
    {
        return API::OpenGL;
    }

    std::unique_ptr<RendererAPI> RendererAPI::Create()
    {
        switch (GetAPI())
        {
        case API::OpenGL:
            return std::make_unique<Engine::Platform::OpenGL::OpenGLRendererAPI>();

        case API::None:
        default:
            CRTN_ASSERT("Unknown API!");
            return nullptr;
        }
    }
}
