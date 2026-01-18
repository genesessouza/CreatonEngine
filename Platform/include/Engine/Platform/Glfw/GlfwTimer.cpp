#include "Engine/Platform/Glfw/GlfwTimer.h"

#include <GLFW/glfw3.h>

namespace Engine::Platform::Glfw
{
    double GlfwGetTimeSeconds()
    {
        return glfwGetTime();
    }
}