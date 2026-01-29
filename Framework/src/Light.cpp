#include "Engine/Framework/Light.h"

#include "Engine/Framework/Scene.h"
#include "Engine/Framework/GameObject.h"

namespace Engine::Framework::Lights
{
    DirectionalLight::DirectionalLight()
        : Engine::Framework::Component(), m_Color(1.00, 0.95, 0.70, 1.0) /* light yellow */, m_Intensity(3.0f), m_Direction(0.0f)
    {
        
    }

    void DirectionalLight::Init()
    {
        GetOwner()->GetTransform().SetPosition(glm::vec3(1.0f, 5.0f, -3.0f));
        GetOwner()->GetTransform().SetScale(glm::vec3(1.0f));
        
        SetDirection(glm::vec3(0.0f, 90.0f, 0.0f));
    }

    void DirectionalLight::OnAddedToScene(Engine::Framework::Scene* scene)
    {
        scene->SetDirectionalLight(this);
    }

    void DirectionalLight::OnRemovedFromScene(Engine::Framework::Scene* scene)
    {
        scene->DeleteDirectionalLight(this);
    }

    PointLight::PointLight()
        : Engine::Framework::Component(), m_Color(1.0f, 1.0f, 1.0f, 1.0f), m_Intensity(10.0f)
    {
        
    }

    void PointLight::Init()
    {
        GetOwner()->GetTransform().SetPosition({ 0.0f, 3.0f, 0.0f });
        GetOwner()->GetTransform().SetScale(glm::vec3(1.0f));
    }

    void PointLight::OnAddedToScene(Engine::Framework::Scene* scene)
    {
        scene->AddPointLight(this);
    }

    void PointLight::OnRemovedFromScene(Engine::Framework::Scene* scene)
    {
        scene->RemovePointLight(this);
    }
}