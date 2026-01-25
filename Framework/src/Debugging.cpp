#include "Engine/Framework/Debugging.h"

#include <Engine/Core/RenderCommand.h>

#include <algorithm>

namespace Engine::Framework
{
    std::vector<Debugging::DebugLine> Debugging::s_Lines;
    std::vector<Debugging::DebugPoint> Debugging::s_Points;

    void Debugging::AddLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, float duration) 
    {
        s_Lines.push_back({ start, end, color, duration });
    }

    void Debugging::AddHitPoint(const glm::vec3& center, float radius, const glm::vec4& color, float duration) 
    {
        s_Points.push_back({ center, radius, color, duration });
    }

    void Debugging::Render(float deltaTime, const glm::mat4& view, const glm::mat4& proj) 
    {
        auto* api = Engine::Core::RenderCommand::GetRendererAPI();

        auto itLine = s_Lines.begin();
        while (itLine != s_Lines.end()) {
            api->DrawLine(itLine->Start, itLine->End, itLine->Color, view, proj);
            itLine->LifeTime -= deltaTime;
            if (itLine->LifeTime <= 0) itLine = s_Lines.erase(itLine);
            else ++itLine;
        }

        auto itPoint = s_Points.begin();
        while (itPoint != s_Points.end()) 
        {
            api->DrawHitpoint(itPoint->Center, itPoint->Radius, itPoint->Color, view, proj);
            itPoint->LifeTime -= deltaTime;
            if (itPoint->LifeTime <= 0) itPoint = s_Points.erase(itPoint);
            else ++itPoint;
        }
    }
}