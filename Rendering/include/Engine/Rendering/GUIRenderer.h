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

		static GUIRenderer* Create();
		static GUIRenderer* Get() { return s_Instance; }
	protected:
		static GUIRenderer* s_Instance;
	};
}