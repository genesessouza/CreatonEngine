#pragma once

#include "Engine/Framework/Component.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <memory>

namespace Engine::Framework
{
	class Entity;
	class Scene;
}

namespace Engine::Framework::Lights
{
	class Light : public Component
	{
	public:
		Light() : Component(), m_Color(1.0f), m_Intensity(1.0f), m_Direction(1.0f) {}
		virtual ~Light() = default;

		virtual void Init() {}

		glm::vec4 GetColor() const { return m_Color; }
		void SetColor(const glm::vec4& color) { m_Color = color; }

		float GetIntensity() const { return m_Intensity; }
		void SetIntensity(float intensity) { m_Intensity = intensity; }

		virtual glm::vec3 GetDirection() const { return m_Direction; }
		virtual void SetDirection(const glm::vec3& dir) { m_Direction = dir; }
	protected:
		glm::vec4 m_Color;
		float m_Intensity;

		glm::vec3 m_Direction;
	};

	class DirectionalLight : public Light
	{
	public:
		DirectionalLight() : Light() {}
		virtual ~DirectionalLight() = default;

		void Init() override;

		static std::unique_ptr<DirectionalLight> Create() { return std::make_unique<DirectionalLight>(); }
	};

	class PointLight : public Light
	{
	public:
		PointLight() : Light() {}
		virtual ~PointLight() = default;

		void Init() override;

		static std::unique_ptr<PointLight> Create() { return std::make_unique<PointLight>(); }
	};
}