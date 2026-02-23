#include "Engine/Core/Layer/LayerStack.h"

#include <Engine/Core/Log/Logger.h>

namespace Engine::Core::Layer
{
	LayerStack::~LayerStack()
	{
		if (!&m_Layers)
		{
			CRTN_LOG_WARNING("<LayerStack::~LayerStack>: m_Layers was already destroyed!");
			return;
		}

		if (m_Layers.size() <= 0)
		{
			CRTN_LOG_INFO("<LayerStack::~LayerStack>: No layers to destroy.");
			return;
		}

		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		if (!&m_Layers)
		{
			CRTN_LOG_CRITICAL("<LayerStack::PushLayer>: [m_Layers] is null pointer!");
			CRTN_ASSERT(!&m_Layers, "<LayerStack::PushLayer>: Layers not initialized!");
			return;
		}

		if (!layer)
		{
			CRTN_LOG_ERROR("<LayerStack::PushLayer>: Pushed layer is null!");
			return;
		}

		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		if (!&m_Layers)
		{
			CRTN_LOG_CRITICAL("<LayerStack::PushOverlay>: [m_Layers] is null pointer!");
			CRTN_ASSERT(!&m_Layers, "<LayerStack::PushLayer>: Layers not initialized!");

			return;
		}

		if (!overlay)
		{
			CRTN_LOG_ERROR("<LayerStack::PushOverlay>: Pushed overlay layer is null!");
			return;
		}

		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		if (!&m_Layers)
		{
			CRTN_LOG_CRITICAL("<LayerStack::PopLayer>: [m_Layers] is null pointer!");
			CRTN_ASSERT(!&m_Layers, "<LayerStack::PushLayer>: Layers not initialized!");
			return;
		}

		if (!layer)
		{
			CRTN_LOG_INFO("<LayerStack::PopLayer>: Layer was already popped.");
			return;
		}

		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
		if (it != m_Layers.begin() + m_LayerInsertIndex)
		{
			layer->OnDetach();
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		if (!&m_Layers)
		{
			CRTN_LOG_CRITICAL("<LayerStack::PopOverlay>: [m_Layers] is null pointer!");
			CRTN_ASSERT(!&m_Layers, "<LayerStack::PushLayer>: Layers not initialized!");

			return;
		}

		if (!overlay)
		{
			CRTN_LOG_INFO("<LayerStack::PopOverlay>: Overlay layer was already popped.");
			return;
		}

		auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
		if (it != m_Layers.end())
		{
			overlay->OnDetach();
			m_Layers.erase(it);
		}
	}
}
