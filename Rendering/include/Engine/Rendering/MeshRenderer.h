#pragma once

#include "Engine/Rendering/MeshGPU.h"
#include "Engine/Rendering/Shader.h"

#include <Engine/Framework/Transform.h>
#include <Engine/Framework/Camera.h>

#include <memory>

namespace Engine::Rendering
{
    class MeshRenderer
    {
    public:
        void Draw(const Engine::Framework::Transform& transform, const Engine::Framework::Camera& camera) const;
    public:
        std::shared_ptr<MeshGPU> m_Mesh;
        std::shared_ptr<Shader> m_Shader;
    };
}