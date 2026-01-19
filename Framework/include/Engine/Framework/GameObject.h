#pragma once

#include "Engine/Framework/Transform.h"

#include <Engine/Rendering/MeshGPU.h>
#include <Engine/Rendering/MeshRenderer.h>

#include <memory>

namespace Engine::Framework
{
	class GameObject
	{
	public:
		GameObject() : m_Renderer(nullptr)
		{
		}

		virtual ~GameObject() = default;

		void Init()
		{
			m_Renderer = Engine::Rendering::MeshRenderer::Create();

			if (m_Renderer)
				m_Renderer->Init();
		}

		inline void Draw() const
		{
			if (m_Renderer)
				m_Renderer->Draw(m_Transform);
		}

		static std::shared_ptr<GameObject> Create() { return std::make_shared<GameObject>(); }
	public:
		Transform m_Transform;
		std::shared_ptr<Engine::Rendering::MeshRenderer> m_Renderer;
	};
}