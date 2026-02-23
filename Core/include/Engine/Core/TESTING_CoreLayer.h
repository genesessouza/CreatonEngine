#pragma once

#include "Engine/Core/Layer/Layer.h"
#include "Engine/Core/Event/Event.h"

namespace Engine::Core
{
	class CoreLayer : public Layer::Layer
	{
	public:
		CoreLayer() : Layer("Core Layer")
		{
			CRTN_LOG_INFO("<CoreLayer::CoreLayer>: Heeeey!");
		}

		~CoreLayer()
		{
		}

		void OnAttach() override
		{

		}

		void OnUpdate(float deltaTime) override
		{

		}

		void OnEvent(Event::Event& e) override
		{

		}
	};
}