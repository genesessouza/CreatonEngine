#include "Engine/Core/Log/Logger.h"

#include "Engine/Core/Window.h"

namespace Engine::Core
{
	static Window::WindowFactory s_Factory = nullptr;

	void Window::SetFactory(WindowFactory factory)
	{
		if (!factory)
		{
			CRTN_LOG_CRITICAL("<Window::SetFactory>: Window Factory is not set!");
			CRTN_ASSERT(!factory, "<Window::SetFactory>: Window factory cannot be null!");

			return;
		}

		s_Factory = factory;
	}

	std::unique_ptr<Window> Window::Create(const WindowProps& props)
	{
		if (!s_Factory)
		{
			CRTN_LOG_CRITICAL("<Window::Create>: [s_Factory] is null ptr!");
			CRTN_ASSERT(!s_Factory, "<Window::Create>: Window factory not set!");

			return nullptr;
		}

		return s_Factory(props);
	}
}