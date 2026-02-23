#pragma once

#include <memory>
#include <functional>

namespace Engine::Core
{
    class RendererAPI;

    class RendererFactory
    {
    public:
        using Creator = std::function<std::unique_ptr<RendererAPI>()>;

        static void RegisterRenderer(Creator creator);

        static void Create();
        static RendererAPI& Get();
        
        template<typename T>
        static bool Is()
        {
            return dynamic_cast<T*>(&RendererFactory::Get()) != nullptr;
        }
    private:
        static Creator s_Creator;
        static std::unique_ptr<RendererAPI> s_Instance;
    };
}