#pragma once

#include "Engine/Editor/GUI/EditorGUI.h"

#include <Engine/Core/Layer/Layer.h>
#include <Engine/Core/Event/KeyEvent.h>
#include <Engine/Core/Event/WindowEvent.h>

#include <Engine/Core/Event/FramebufferEvent.h>

#include <Engine/Framework/Scene.h>

namespace Engine::Editor
{
	class EditorLayer : public Engine::Core::Layer::Layer
	{
	public:
		EditorLayer() : Layer("Editor")
		{
			m_EditorGUI.Init();
		}

		void OnAttach() override { }

		void OnUpdate(float deltaTime) override
		{

		}
		
		void OnEvent(Engine::Core::Event::Event& e) override
		{

		}

	private:
		EditorGUI m_EditorGUI;
	};
}