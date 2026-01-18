#pragma once

#include "Engine/Core/Event/Event.h"

#include <sstream>

namespace Engine::Core::Event
{
    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(float x, float y)
            : m_MouseX(x), m_MouseY(y) {
        }

        float GetX() const { return m_MouseX; }
        float GetY() const { return m_MouseY; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "Mouse Position - X: [" << m_MouseX << "] | Y: [" << m_MouseY << "]";
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventInput | EventMouse)
    private:
        float m_MouseX, m_MouseY;
    };

    class MouseClickedEvent : public Event
    {
    public:
        MouseClickedEvent(int button)
            : m_Button(button) {
		}

        int GetMouseButton() const { return m_Button; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "Mouse Button: [" << m_Button << "] clicked!";
            return ss.str();
        }

		EVENT_CLASS_TYPE(MouseButtonPressed)
        EVENT_CLASS_CATEGORY(EventInput | EventMouse | EventMouseButton)
    protected:
        int m_Button;
    };

    class MouseScrolledEvent : public Event
    {
        public:
        MouseScrolledEvent(float xOffset, float yOffset)
            : m_XOffset(xOffset), m_YOffset(yOffset) {
        }

        float GetXOffset() const { return m_XOffset; }
        float GetYOffset() const { return m_YOffset; }
        
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "Mouse Scrolled - X Offset: [" << m_XOffset << "] | Y Offset: [" << m_YOffset << "]";
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventInput | EventMouse)
    private:
		float m_XOffset, m_YOffset;
    };
}