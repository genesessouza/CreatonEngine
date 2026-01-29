#pragma once

#include "Engine/Editor/GUI/EditorGUI.h"
#include "Engine/Editor/EditorStyle.h"

#include <Engine/Core/Layer/Layer.h>
#include <Engine/Core/Log/Logger.h>

namespace Engine::Editor
{
	class EditorLayer : public Engine::Core::Layer::Layer
	{
	public:
		EditorLayer() : Layer("Editor"), m_EditorGUI(nullptr)
		{
			CRTN_LOG_INFO("[EDITOR LAYER]: Started editor");
			m_EditorGUI = new EditorGUI();
		}

		~EditorLayer()
		{
			m_EditorGUI->OnShutdown();
		}

		void OnAttach() override
		{
			m_EditorGUI->OnInit();

			m_EditorGUI->SetResizeCallback([this](uint32_t w, uint32_t h)
				{
					Engine::Core::Event::FramebufferResizeEvent e(w, h);
					this->m_EventCallback(e);
				});

			Engine::Editor::EditorStyle::ApplyDarkTheme();
		}

		void OnUpdate(float deltaTime) override
		{
		}

		void OnEditorUpdate(float deltaTime) override
		{

		}

		void OnGUIUpdate() override
		{
			m_EditorGUI->OnGUIRender();
		}

		void OnEvent(Engine::Core::Event::Event& e) override
		{
		}
	private:
		EditorGUI* m_EditorGUI;
	};
}