#include "Engine/Core/Log/Logger.h"

#include "Engine/Core/Window.h"

namespace Engine::Core
{
    static Window::WindowFactory s_Factory = nullptr;

    void Window::SetFactory(WindowFactory factory)
    {
		CRTN_ASSERT(factory, "Window factory cannot be null!");
        s_Factory = factory;
    }

    std::unique_ptr<Window> Window::Create(const WindowProps& props)
    {
        CRTN_ASSERT(s_Factory, "Window factory not set!");
        return s_Factory(props);
    }
}