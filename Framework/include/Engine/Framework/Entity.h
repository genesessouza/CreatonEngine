#pragma once

#include "Engine/Framework/Transform.h"

#include <string>

namespace Engine::Framework
{
    class Entity 
    {
    public:
        Entity(const std::string& name = "Entity")
            : m_Name(name), m_Enabled(true) {
        }

        virtual ~Entity() = default;

        void SetEnabled(bool enabled) { m_Enabled = enabled; }
        bool IsEnabled() const { return m_Enabled; }

        const std::string& GetName() const { return m_Name; }
        void SetName(const std::string& name) { m_Name = name; }

        Transform& GetTransform() { return m_Transform; }
    protected:
        std::string m_Name;
        bool m_Enabled;
        Transform m_Transform;
    };
}