#pragma once

#include <string>

namespace Engine::Core::Event
{
    enum class EventType
    {
        None = 0,

        WindowClose, WindowMaximize, WindowMove, WindowResize,
        KeyPressed, KeyReleased,
        MouseButtonPressed, MouseButtonReleased,
        MouseMoved, MouseScrolled
    };

    enum EventCategory
    {
        None = 0,
        EventWindow = 1 << 0,
        EventInput = 1 << 1,
        EventKeyboard = 1 << 2,
        EventMouse = 1 << 3,
        EventMouseButton = 1 << 4
    };

    #define EVENT_CLASS_TYPE(type) \
        static EventType GetStaticType() { return EventType::type; } \
        virtual EventType GetType() const override { return GetStaticType(); } \
        virtual const char* GetName() const override { return #type; }

    #define EVENT_CLASS_CATEGORY(category) \
        virtual int GetCategoryFlags() const override { return category; }

    class Event
    {
    public:
        Event() = default;
        virtual ~Event() = default;

        virtual EventType GetType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;

        bool Handled = false;

        bool IsInCategory(EventCategory category) const
        {
            return GetCategoryFlags() & category;
        }

        virtual std::string ToString() const
        {
            return GetName();
        }
    };

    class EventDispatcher
    {
    public:
        EventDispatcher(Event& event) : m_Event(event) {}

        template<typename T, typename F>
        bool Dispatch(const F& func)
        {
            if (m_Event.GetType() == T::GetStaticType())
            {
                m_Event.Handled = func(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }

    private:
        Event& m_Event;
    };
}

#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }