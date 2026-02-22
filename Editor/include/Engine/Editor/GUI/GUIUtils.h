#pragma once

#include <glm/vec2.hpp>

namespace Engine::Editor
{
	class EditorGUI;
}

namespace Engine::Editor::GUI
{
	class GUIUtils
	{
	public:
        static glm::vec2 GetMousePos();
        static glm::vec2 GetMouseRelativePos();

		static bool IsMouseInsideViewport();

        static glm::vec2 GetViewportMin();
        static glm::vec2 GetViewportSize();
	};
}