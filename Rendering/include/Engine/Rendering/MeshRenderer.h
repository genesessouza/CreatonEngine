#pragma once

#include "Engine/Rendering/MeshGPU.h"
#include "Engine/Rendering/Shader.h"
#include "Engine/Rendering/Material.h"

#include <Engine/Framework/Transform.h>
#include <Engine/Framework/Camera.h>

#include <memory>

namespace Engine::Rendering
{
    class MeshRenderer
    {
    public:
        MeshRenderer()
            : m_MeshMat(nullptr)
        {
        }

        virtual ~MeshRenderer() = default;

        void Init()
        {
            m_MeshMat = Engine::Rendering::Material::Create();

            if (m_MeshMat)
                m_MeshMat->Init();
        }

        void Draw(const Engine::Framework::Transform& transform) const;

        static std::shared_ptr<MeshRenderer> Create() 
        { 
            auto meshRenderer = std::make_shared<MeshRenderer>(); 
            CRTN_CHECK_PTR(meshRenderer);

            return meshRenderer;
        }
    public:
        std::shared_ptr<MeshGPU> m_Mesh;
        std::shared_ptr<Material> m_MeshMat;
    };
}