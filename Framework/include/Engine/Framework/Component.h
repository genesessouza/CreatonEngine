#pragma once

namespace Engine::Framework
{
    class GameObject;
    class Scene;

    class Component
    {
    public:
        Component() = default;
        virtual ~Component() = default;

        virtual void Init() {}

        virtual void OnUpdate() {}

        bool IsEnabled() const { return m_IsEnabled; }
        void SetEnabled(bool value) { m_IsEnabled = value; }

        virtual void SetOwner(Engine::Framework::GameObject* owner) { m_Owner = owner; };
        virtual Engine::Framework::GameObject* GetOwner() const { return m_Owner; };

        virtual void OnAddedToScene(Engine::Framework::Scene* scene) {};
        virtual void OnRemovedFromScene(Engine::Framework::Scene* scene) {};
    protected:
        Engine::Framework::GameObject* m_Owner = nullptr;

        bool m_IsEnabled = true;
    };
}