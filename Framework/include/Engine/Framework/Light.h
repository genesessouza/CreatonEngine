#pragma once

#include "Engine/Framework/GameObject.h"

namespace Engine::Framework::Lights
{
    class DirectionalLight : public Entity
    {
    public:
        DirectionalLight() : Entity("Directional Light"), m_Color(1.00, 0.95, 0.70, 1.0) /* light yellow */, m_Intensity(3.0f)
        {
            m_Transform.SetPosition(glm::vec3(1.0f, 5.0f, -3.0f));
        }

        virtual ~DirectionalLight() = default;

        glm::vec4 GetColor() const { return m_Color * m_Intensity; }
        void SetColor(const glm::vec4& color, float intensity) { m_Color = color * intensity; }

        float GetIntensity() const { return m_Intensity; }
        void SetIntensity(float intensity) { m_Intensity = intensity; }

        static std::shared_ptr<DirectionalLight> Create() { return std::make_shared<DirectionalLight>(); }
    private:
        glm::vec4 m_Color;
        float m_Intensity;
    };

    class PointLight : public Entity 
    {
    public:
        PointLight() : Entity("Point Light"), m_Color(1.0f), m_Intensity(10.0f) 
        {
			m_Transform.SetPosition({ 0.0f, 3.0f, 0.0f });
        }

        glm::vec4 GetColor() const { return m_Color; }
        void SetColor(const glm::vec4& color) { m_Color = color; }

        float GetIntensity() const { return m_Intensity; }
        void SetIntensity(float intensity) { m_Intensity = intensity; }

		static std::shared_ptr<PointLight> Create() { return std::make_shared<PointLight>(); }
    private:
        glm::vec4 m_Color;
        float m_Intensity;
    };
}