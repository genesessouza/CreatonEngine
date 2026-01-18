#pragma once

#include "Engine/Core/Event/Event.h"
#include "Engine/Core/Time/Time.h"

#include <string>

namespace Engine::Core::Layer
{
    class Layer
    {
    public:
        Layer(const std::string& name = "Layer")
            : m_DebugName(name) {
        }

        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}

        virtual void OnUpdate(float deltaTime) = 0;
        virtual void OnEvent(Event::Event& event) = 0;

        const std::string& GetName() const { return m_DebugName; }

    protected:
        std::string m_DebugName;
    };
}
