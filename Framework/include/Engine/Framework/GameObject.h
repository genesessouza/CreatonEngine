#pragma once

#include "Engine/Framework/Entity.h"
#include "Engine/Framework/Component.h"
#include "Engine/Framework/Scene.h"

#include <Engine/Rendering/MeshGPU.h>
#include <Engine/Rendering/MeshRenderer.h>

#include <memory>
#include <string>

namespace Engine::Framework
{
    class GameObject : public Entity
    {
    public:
        GameObject(const char* name) 
            : Entity(name) 
        {
        }

        virtual ~GameObject() = default;

        void OnUpdate() override
        {
            for (auto& comp : m_Components)
                comp->OnUpdate();
        }

        const std::string& GetName() const { return m_Name; }
        void SetName(const std::string& name) { m_Name = name; }

        Engine::Framework::Transform& GetTransform() { return m_Transform; }
        const Engine::Framework::Transform& GetTransform() const { return m_Transform; }

        template<typename T>
        T* GetComponent()
        {
            if constexpr (std::is_same_v<T, Transform>)
                return &m_Transform;

            for (auto& c : m_Components)
            {
                if (auto casted = dynamic_cast<T*>(c.get()))
                    return casted;
            }

            return nullptr;
        }

        template<typename T, typename... Args>
        T* AddComponent(Args&&... args)
        {
            static_assert(std::is_base_of_v<Component, T>,
                "T must derive from Component");

            static_assert(!std::is_abstract_v<T>,
                "Cannot instantiate abstract Component");

            auto comp = std::make_unique<T>(std::forward<Args>(args)...);
            T* raw = comp.get();
            raw->SetOwner(this);

            m_Components.push_back(std::move(comp));

            if (m_Scene)
                raw->OnAddedToScene(m_Scene);

            return raw;
        }

        static std::unique_ptr<GameObject> Create(const char* name) { return std::make_unique<GameObject>(name); }
    private:
        std::vector<std::unique_ptr<Engine::Framework::Component>> m_Components;

        Engine::Framework::Scene* m_Scene = nullptr;
    };
}