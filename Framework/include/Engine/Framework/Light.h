#pragma once

#include "Engine/Framework/Component.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <memory>

namespace Engine::Framework
{
    class Scene;
    class GameObject;
}

namespace Engine::Framework::Lights
{
    class DirectionalLight : public Engine::Framework::Component
    {
    public:
        DirectionalLight();
        virtual ~DirectionalLight() = default;

        virtual void Init() override;

        glm::vec4 GetColor() const { return m_Color * m_Intensity; }
        void SetColor(const glm::vec4& color, float intensity) { m_Color = color * intensity; }

        float GetIntensity() const { return m_Intensity; }
        void SetIntensity(float intensity) { m_Intensity = intensity; }

        glm::vec3 GetDirection() const { return m_Direction; }
        void SetDirection(const glm::vec3& dir) { m_Direction = dir; }

        void OnAddedToScene(Engine::Framework::Scene* scene) override;
        void OnRemovedFromScene(Engine::Framework::Scene* scene) override;

        static std::unique_ptr<DirectionalLight> Create() { return std::make_unique<DirectionalLight>(); }
    private:
        glm::vec4 m_Color;
        float m_Intensity;

        glm::vec3 m_Direction;
    };

    class PointLight : public Engine::Framework::Component
    {
    public:
        PointLight();
        virtual ~PointLight() = default;

        virtual void Init() override;

        glm::vec4 GetColor() const { return m_Color; }
        void SetColor(const glm::vec4& color) { m_Color = color; }

        float GetIntensity() const { return m_Intensity; }
        void SetIntensity(float intensity) { m_Intensity = intensity; }

        void OnAddedToScene(Engine::Framework::Scene* scene) override;
        void OnRemovedFromScene(Engine::Framework::Scene* scene) override;

        static std::unique_ptr<PointLight> Create() { return std::make_unique<PointLight>(); }
    private:
        glm::vec4 m_Color;
        float m_Intensity;
    };
}