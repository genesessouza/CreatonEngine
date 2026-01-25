#include "Engine/Editor/GUI/GUIUtils.h"

#include <ImGui/imgui.h>

#include "Engine/Editor/GUI/EditorGUI.h"

namespace Engine::Editor::GUI 
{
    glm::vec2 GUIUtils::GetMousePos() 
    {
        ImVec2 pos = ImGui::GetMousePos();
        return { pos.x, pos.y };
    }

    glm::vec2 GUIUtils::GetMouseRelativePos() 
    {
        glm::vec2 mPos = GetMousePos();
        glm::vec2 vMin = GetViewportMin();
        return mPos - vMin;
    }

    bool GUIUtils::IsMouseInsideViewport() 
    {
        glm::vec2 mPos = GetMousePos();
        glm::vec2 vMin = GetViewportMin();
        glm::vec2 vSize = GetViewportSize();

        return (mPos.x >= vMin.x && mPos.x <= vMin.x + vSize.x &&
            mPos.y >= vMin.y && mPos.y <= vMin.y + vSize.y);
    }

    glm::vec2 GUIUtils::GetViewportMin() 
    {
        auto bounds = Engine::Editor::EditorGUI::Get().GetViewportBounds();
        return { bounds[0].x, bounds[0].y };
    }

    glm::vec2 GUIUtils::GetViewportSize() 
    {
        auto bounds = Engine::Editor::EditorGUI::Get().GetViewportBounds();
        return { bounds[1].x - bounds[0].x, bounds[1].y - bounds[0].y };
    }
}