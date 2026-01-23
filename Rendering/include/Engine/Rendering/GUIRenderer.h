#pragma once

#include <memory>

namespace Engine::Rendering
{
	class GUIRenderer
	{
	public:
		virtual ~GUIRenderer() = default;
		
		virtual void ClearUI() = 0;

		virtual void Begin() = 0;
		virtual void End() = 0;

		static std::unique_ptr<GUIRenderer> Create();
	};
}