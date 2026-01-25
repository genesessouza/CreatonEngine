#pragma once

#include "Engine/Framework/Transform.h"
#include "Engine/Framework/Entity.h"
#include "Engine/Framework/Component.h"

#include <Engine/Rendering/MeshGPU.h>
#include <Engine/Rendering/MeshRenderer.h>

#include <memory>

namespace Engine::Framework
{
    class GameObject : public Entity 
    {
    public:
        GameObject(const std::string& name = "GameObject") : Entity(name)
        {
			m_Transform.SetPosition({0.0f, 0.0f, 0.0f});
			m_Transform.SetRotation({ 0.0f, 0.0f, 0.0f });
			m_Transform.SetScale({ 1.0f, 1.0f, 1.0f });
        }

        void Init() 
        {
            m_MeshRenderer = Engine::Rendering::MeshRenderer::Create();
            m_MeshRenderer->Init();
        }

        void Draw() 
        {
            if (m_Enabled && m_MeshRenderer)
                m_MeshRenderer->Draw(m_Transform);
        }

        template<typename T, typename... Args>
        std::shared_ptr<T> AddComponent(Args&&... args) 
        {
            auto comp = std::make_shared<T>(this, std::forward<Args>(args)...);
            m_Components.push_back(comp);
            return comp;
        }

        template<typename T>
        std::shared_ptr<T> GetComponent() 
        {
            for (auto& comp : m_Components) 
            {
                auto casted = std::dynamic_pointer_cast<T>(comp);
                if (casted) return casted;
            }
            return nullptr;
        }

        auto& GetMeshRenderer() { return m_MeshRenderer; }

        static std::shared_ptr<GameObject> Create(const std::string& name = "GameObject") 
        { 
            auto gameObject = std::make_shared<GameObject>(name); 
            CRTN_CHECK_PTR(gameObject);
            return gameObject;
		}
    private:
        std::shared_ptr<Engine::Rendering::MeshRenderer> m_MeshRenderer;
        std::vector<std::shared_ptr<Component>> m_Components;
    };
}