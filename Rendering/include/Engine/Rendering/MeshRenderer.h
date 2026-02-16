#pragma once

#include "Engine/Rendering/Material.h"
#include <Engine/Framework/Component.h>

#include <memory>

namespace Engine::Framework
{
    class Transform;
    class Scene;
    class Entity;
    class Mesh;
}

namespace Engine::Rendering
{
    class MeshRenderer : public Engine::Framework::Component
    {
    public:
        struct MeshUniformLocations
        {
            int Model = -1;
            int Normal = -1;
        };

        MeshRenderer() = default;
        virtual ~MeshRenderer() = default;

        void Init()
        {
            if (!m_MeshMat)
            {
                m_MeshMat = Material::Create();
                m_MeshMat->Init();
            }
        }

        void InitUniforms();

        void Draw(Engine::Framework::Transform& transform) const;

        void SetMesh(const std::shared_ptr<MeshGPU>& mesh)
        {
            m_Mesh = mesh;
        }

        const std::shared_ptr<MeshGPU>& GetMesh() const { return m_Mesh; }

        const std::shared_ptr<Material>& GetMaterial() const { return m_MeshMat; }
    private:
        std::shared_ptr<MeshGPU> m_Mesh;
        std::shared_ptr<Material> m_MeshMat;

        MeshUniformLocations m_MeshUniforms;
        bool m_UniformsInitialized = false;
    };
}