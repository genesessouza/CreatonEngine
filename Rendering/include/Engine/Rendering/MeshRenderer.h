#pragma once

#include "Engine/Rendering/Material.h"

#include <Engine/Framework/Component.h>

#include <memory>

namespace Engine::Framework
{
    class Transform;
    class Scene;
    class GameObject;
}

namespace Engine::Rendering
{
    class Renderer;
    class MeshGPU;

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
                m_MeshMat = Engine::Rendering::Material::Create();
                m_MeshMat->Init();
            }
        }

        void InitUniforms();

        void OnAddedToScene(Engine::Framework::Scene* scene) override;
        void OnRemovedFromScene(Engine::Framework::Scene* scene) override;

        void Draw(Engine::Framework::Transform& transform) const;

        void SetMesh(const std::shared_ptr<Engine::Rendering::MeshGPU>& mesh)
        {
            m_Mesh = mesh;
        }

        const std::shared_ptr<Engine::Rendering::MeshGPU>& GetMesh() const { return m_Mesh; }

        const std::shared_ptr<Engine::Rendering::Material>& GetMaterial() const { return m_MeshMat; }
    private:
        std::shared_ptr<Engine::Rendering::MeshGPU> m_Mesh;
        std::shared_ptr<Engine::Rendering::Material> m_MeshMat;

        MeshUniformLocations m_MeshUniforms;
        bool m_UniformsInitialized = false;
    };
}