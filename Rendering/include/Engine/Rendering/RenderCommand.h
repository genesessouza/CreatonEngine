#pragma once

#include <glm/glm.hpp>

namespace Engine::Rendering
{
    class MeshGPU;
    class Material;

    struct RenderCommand
    {
        MeshGPU* MeshPtr;
        Material* MaterialPtr;
        glm::mat4 Transform;
    };
}