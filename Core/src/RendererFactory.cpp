#include "Engine/Core/RendererFactory.h"

#include "Engine/Core/RendererAPI.h"

#include <Engine/Core/Log/Logger.h>

#include <stdexcept>

namespace Engine::Core
{
    RendererFactory::Creator RendererFactory::s_Creator = nullptr;
    std::unique_ptr<RendererAPI> RendererFactory::s_Instance = nullptr;

    void RendererFactory::RegisterRenderer(Creator creator)
    {
        if (!creator)
        {
            CRTN_LOG_CRITICAL("<RendererFactory::RegisterRenderer>: Passed RendererAPI is not registered!");
            CRTN_ASSERT(!creator, "<RendererFactory::RegisterRenderer>: Failed to register new RendererAPI!");
            return;
        }

        s_Creator = std::move(creator);
    }

    void RendererFactory::Create()
    {
        if (!s_Creator)
        {
            CRTN_LOG_CRITICAL("<RendererFactory::Create>: RendererAPI not registered!");
            CRTN_ASSERT(!s_Creator, "<RendererFactory::Create>: Failed to create RendererAPI!");
        }

        s_Instance = s_Creator();
    }

    RendererAPI& RendererFactory::Get()
    {
        if (!s_Instance)
        {
            CRTN_LOG_CRITICAL("<RendererFactory::Get>: RendererAPI not initialized!");
            CRTN_ASSERT(!s_Instance, "Could not retrieve RendererAPI!");
        }

        return *s_Instance;
    }
}