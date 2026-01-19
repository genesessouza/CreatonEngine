#pragma once

#include "Engine/Framework/GameObject.h"

namespace Engine::Framework::Lights
{
    class DirectionalLight : public GameObject
    {
    public:
        DirectionalLight()
        {
            m_Direction = glm::vec3(4, 7, 3);
            m_Color = glm::vec4(1.00, 0.95, 0.70, 1.0);
            m_Intensity = 1.5f;
        }

        virtual ~DirectionalLight() = default;

        glm::vec3 GetDirection() const { return m_Direction; }
        void SetDirection(const glm::vec3& direction) { m_Direction = direction; }

        glm::vec4 GetColor() const { return m_Color * m_Intensity; }
        void SetColor(const glm::vec4& color, float intensity) { m_Color = color * intensity; }

        static std::shared_ptr<DirectionalLight> Create() { return std::make_shared<DirectionalLight>(); }
    private:
        glm::vec3 m_Direction;
        glm::vec4 m_Color;

        float m_Intensity;
    };

    class PointLight : public GameObject
    {
    public:
        PointLight()
        {
            m_Color = glm::vec4(1.00, 0.95, 0.70, 1.0);
            m_Intensity = 10.0f;
        }

        virtual ~PointLight() = default;

        const glm::vec4& GetColor() const { return m_Color; }
        void SetColor(const glm::vec4& color) { m_Color = color; }

        float GetIntensity() const { return m_Intensity; }
        void SetIntensity(float intensity) { m_Intensity = intensity; }

        static std::shared_ptr<PointLight> Create() { return std::make_shared<PointLight>(); }
    private:
        glm::vec4 m_Color;
        float m_Intensity;
    };
}