#include "Engine/Framework/Raycast.h"
#include "Engine/Framework/Entity.h"
#include "Engine/Framework/Camera.h"
#include "Engine/Framework/Debugging.h"
#include "Engine/Framework/Scene.h"

#include "Engine/Framework/Physics/Collider.h"

#include <Engine/Editor/GUI/GUIUtils.h>

namespace Engine::Framework
{
    Raycast::RayResult Raycast::MouseToWorldPos(std::shared_ptr<Camera> camera,bool debug)
    {
        RayResult result;
        result.Distance = std::numeric_limits<float>::max();

        if (!Engine::Editor::GUI::GUIUtils::IsMouseInsideViewport()) return result;

        glm::vec2 relativeMousePos = Engine::Editor::GUI::GUIUtils::GetMouseRelativePos();
        glm::vec2 vSize = Engine::Editor::GUI::GUIUtils::GetViewportSize();

        float ndcX = (2.0f * relativeMousePos.x) / vSize.x - 1.0f;
        float ndcY = 1.0f - (2.0f * relativeMousePos.y) / vSize.y;

        glm::vec4 rayClip = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
        glm::vec4 rayView = glm::inverse(camera->GetProjectionMatrix()) * rayClip;
        rayView = glm::vec4(rayView.x, rayView.y, -1.0f, 0.0f);

        glm::vec3 rayDir = glm::normalize(glm::vec3(glm::inverse(camera->GetViewMatrix()) * rayView));
        glm::vec3 rayOrigin = camera->GetTransform().GetPosition();

		Scene& scene = Scene::Get();
        
        for (auto& entity : scene.GetEntities())
        {
            if (!entity->IsEnabled()) continue;

            float dist = 0.0f;
            bool hit = false;

            if (auto obj = std::dynamic_pointer_cast<GameObject>(entity))
                hit = Physics::Collider::IntersectsOBB(rayOrigin, rayDir, *entity, dist);
            else if (auto light = std::dynamic_pointer_cast<Lights::PointLight>(entity))
                hit = Physics::Collider::IntersectsSphere(rayOrigin, rayDir, *entity, 0.3f, dist);

            if (hit && dist < result.Distance) {
                result.Success = true;
                result.HitEntity = entity;
                result.Distance = dist;
            }
        }

        if (result.Success)
        {
            result.HitPoint = rayOrigin + (rayDir * result.Distance);
        }

        if (debug)
        {
            glm::vec3 debugEndPoint = result.Success ? result.HitPoint : rayOrigin + (rayDir * 50.0f);
            Debugging::AddLine(rayOrigin, debugEndPoint, { 1.0f, 0.0f, 0.0f, 1.0f }, 3.0f);

            if (result.Success)
                Debugging::AddHitPoint(result.HitPoint, 0.05f, { 0.0f, 1.0f, 0.0f, 1.0f }, 3.0f);
        }

        return result;
    }
}