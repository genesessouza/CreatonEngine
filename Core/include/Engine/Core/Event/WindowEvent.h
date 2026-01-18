#pragma once

#include "Engine/Core/Event/Event.h"

#include <sstream>

namespace Engine::Core::Event
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height)
			: m_Width(width), m_Height(height) {
		}

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Window Size - X: [" << m_Width << "] | Y: [" << m_Height << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventWindow)
	private:
		uint32_t m_Width, m_Height;
	};

	class WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(uint32_t x, uint32_t y)
			: m_WindowX(x), m_WindowY(y) {
		}

		uint32_t GetX() const { return m_WindowX; }
		uint32_t GetY() const { return m_WindowY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Window Position - X: [" << m_WindowX << "] | Y: [" << m_WindowY << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowMove)
		EVENT_CLASS_CATEGORY(EventWindow)
	private:
		uint32_t m_WindowX, m_WindowY;
	};

	class WindowMaximizeEvent : public Event
	{
	public:
		WindowMaximizeEvent(int maximized)
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Window maximized";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowMaximize)
		EVENT_CLASS_CATEGORY(EventWindow)
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Window closed";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventWindow)
	};
}