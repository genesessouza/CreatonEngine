#pragma once

#include "Engine/Core/Event/Event.h"
#include "Engine/Core/Time/Time.h"

#include <string>
#include <functional>

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

        using EventCallbackFn = std::function<void(Event::Event&)>;

        void SetEventCallback(const EventCallbackFn& callback) { m_EventCallback = callback; }

        virtual void OnUpdate(float deltaTime) = 0;

        virtual void OnGUIUpdate() = 0;
        
        virtual void OnEvent(Event::Event& event) = 0;

        const std::string& GetName() const { return m_DebugName; }
    protected:
        std::string m_DebugName;
		EventCallbackFn m_EventCallback;
    };
}
