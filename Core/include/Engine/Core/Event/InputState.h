#pragma once

#include <glm/vec2.hpp>

#include <cstdint>

namespace Engine::Core::Event::State
{
    struct MouseState
    {
        glm::vec2 Position{ 0.0f };
        glm::vec2 DeltaPos{ 0.0f };

        float ScrollDelta = 0.0f;

        bool Moved = false;
        bool Scrolled = false;
    };

    struct WindowState
    {
        uint32_t Width = 0;
        uint32_t Height = 0;

        uint32_t PosX = 0;
        uint32_t PosY = 0;

        bool Resized = false;
        bool Moved = false;
    };
}