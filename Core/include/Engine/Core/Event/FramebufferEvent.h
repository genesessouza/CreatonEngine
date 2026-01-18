#pragma once

#include "Engine/Core/Event/Event.h"

#include <sstream>

namespace Engine::Core::Event
{
	class FramebufferResizeEvent : public Event
	{
	public:
		FramebufferResizeEvent(uint32_t width, uint32_t height)
			: m_Width(width), m_Height(height) {
		}

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Framebuffer Size - X: [" << m_Width << "] | Y: [" << m_Height << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventWindow)
	private:
		uint32_t m_Width, m_Height;
	};
}