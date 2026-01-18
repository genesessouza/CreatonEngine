#pragma once

#include "Engine/Framework/Transform.h"

#include <Engine/Rendering/MeshGPU.h>
#include <Engine/Rendering/MeshRenderer.h>

#include <memory>

namespace Engine::Framework
{
    class GameObject
    {
    public:
        inline void Draw(const Camera& camera) const
        {
            //CRTN_LOG_INFO("GameObject::Draw() called");

            if (m_Renderer)
                m_Renderer->Draw(m_Transform, camera);
        }
    public:
        Transform m_Transform;
        std::unique_ptr<Engine::Rendering::MeshRenderer> m_Renderer;
    };
}