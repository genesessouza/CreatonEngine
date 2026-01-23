#pragma once

#include <cstdint>

namespace Engine::Core
{
	class RenderCommand 
	{
	public:
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void Clear();

		static void MarkFramebufferDirty();
		
		static void ClearUI();
		static void BeginGUI();
		static void EndGUI();
	};
}