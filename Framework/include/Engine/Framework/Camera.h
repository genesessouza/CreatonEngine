#pragma once

#include "Engine/Framework/Component.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <string>
#include <memory>

namespace Engine::Framework
{
	class Entity;
	class Transform;

	class Camera : public Component
	{
	public:
		Camera() : Component(), m_AspectRatio(16.0f / 9.0f), m_FOV(60.0f), m_FarPlane(100.0f), m_NearPlane(0.1f) {}
		virtual ~Camera() = default;

		void Init() override;

		const glm::mat4& GetViewMatrix() const;
		const glm::mat4& GetProjectionMatrix() const;
		glm::mat4 GetViewProjectionMatrix() const;

		void SetViewportSize(uint32_t width, uint32_t height);

		const void SetFOV(float newFOV) { m_FOV = newFOV; }
		const float GetFOV() const { return m_FOV; }

		const void SetNear(float newNearPlane) { m_NearPlane = newNearPlane; }
		float GetNear() const { return m_NearPlane; }

		const void SetFar(float newFarPlane) { m_FarPlane = newFarPlane; }
		float GetFar() const { return m_FarPlane; }

		static std::unique_ptr<Camera> Create() { return std::make_unique<Camera>(); }
	private:
		float m_FOV;
		float m_AspectRatio;
		float m_NearPlane;
		float m_FarPlane;
	private:
		mutable glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		mutable glm::mat4 m_CachedViewMatrix = glm::mat4(1.0f);
	};
}