#pragma once

#include "Engine/Editor/GUI/EditorGUI.h"
#include "Engine/Editor/EditorStyle.h"

#include <Engine/Core/Layer/Layer.h>
#include <Engine/Core/Log/Logger.h>

#include <Engine/Core/Event/FramebufferEvent.h>

namespace Engine::Editor
{
	class EditorLayer : public Engine::Core::Layer::Layer
	{
	public:
		EditorLayer() : Layer("Editor"), m_EditorGUI(nullptr)
		{
			CRTN_LOG_TRACE("<EditorLayer::EditorLayer>: Started editor");
			m_EditorGUI = new EditorGUI();

			if (!m_EditorGUI)
			{
				CRTN_LOG_CRITICAL("<EditorLayer::EditorLayer>: Failed to initialize EditorLayer's Editor reference!");
				CRTN_ASSERT(!m_EditorGUI, "<EditorLayer::EditorLayer>: Editor's EditorGUI* is null!");

				return;
			}
		}

		~EditorLayer()
		{
			if (!m_EditorGUI)
			{
				CRTN_LOG_CRITICAL("<EditorGUI::~EditorGUI>: EditorGUI is already destroyed!");
				CRTN_ASSERT(!m_EditorGUI, "<EditorGUI::~EditorGUI>: Failed to destroy EditorGUI!");

				return;
			}

			m_EditorGUI->OnShutdown();
		}

		void OnAttach() override
		{
			m_EditorGUI->OnInit();

			m_EditorGUI->SetResizeCallback([this](uint32_t w, uint32_t h)
				{
					Engine::Core::Event::FramebufferResizeEvent e(w, h);
					this->m_EventCallback(e);
					//CRTN_LOG_TRACE("ImGui FBO Resize Event: %s", e.ToString().c_str());
				});

			Engine::Platform::GUI::ImGuiRenderer::Create();
			EditorStyle::ApplyDarkTheme();
		}

		void OnUpdate(float deltaTime) override
		{
			Engine::Platform::GUI::ImGuiRenderer::Begin();
			m_EditorGUI->OnGUIRender();
			Engine::Platform::GUI::ImGuiRenderer::End();
		}

		void OnEvent(Engine::Core::Event::Event& e) override
		{
		}
	private:
		EditorGUI* m_EditorGUI;
	};
}