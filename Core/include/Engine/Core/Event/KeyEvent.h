#include "Engine/Core/Event/Event.h"

#include <sstream>

namespace Engine::Core::Event
{
    class KeyEvent : public Event
    {
    public:
        int GetKeyCode() const { return m_KeyCode; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "Key: [" << (char)m_KeyCode << "] pressed!";
            return ss.str();
        }

        EVENT_CLASS_CATEGORY(EventInput | EventKeyboard)
    protected:
        KeyEvent(int keycode) : m_KeyCode(keycode) {}
        int m_KeyCode;
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(int keycode, int repeatCount)
            : KeyEvent(keycode), m_RepeatCount(repeatCount++) {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "Key: [" << (char)m_KeyCode << "] pressed " << "(" << m_RepeatCount << ") times!";
            return ss.str();
        }

        int GetRepeatCount() const { return m_RepeatCount; }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int m_RepeatCount;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(int keycode)
            : KeyEvent(keycode) {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "Key: [" << (char)m_KeyCode << "] released!";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };
}