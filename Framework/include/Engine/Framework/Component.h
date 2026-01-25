#pragma once

#include "Engine/Framework/Entity.h"

namespace Engine::Framework
{
    enum class ComponentType { Physics, MeshRenderer, Script, Collider };

    class Component 
    {
    public:
        Component(Entity* owner) : m_Owner(owner) {}
        virtual ~Component() = default;

        virtual ComponentType GetType() const = 0;

		Entity* GetOwner() const { return m_Owner; }

		void SetEnabled(bool enabled) { m_Enabled = enabled; }
		bool IsEnabled() const { return m_Enabled; }
    protected:
        Entity* m_Owner;
		bool m_Enabled = true;
    };
}