#pragma once

namespace Engine::Framework
{
    class Entity;
    class Scene;

    class Component
    {
    public:
        Component() = default;
        virtual ~Component() = default;

        virtual void Init() {}

        bool IsEnabled() const { return m_IsEnabled; }
        void SetEnabled(bool value) { m_IsEnabled = value; }

        virtual void SetOwner(Entity* owner) { m_Owner = owner; }
        virtual Entity* GetOwner() const { return m_Owner; }

        virtual void OnUpdate() {}
    protected:
        Entity* m_Owner = nullptr;

        bool m_IsEnabled = true;
    };
}