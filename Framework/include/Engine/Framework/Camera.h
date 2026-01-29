#pragma once

#include "Engine/Framework/Entity.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <string>
#include <memory>

namespace Engine::Framework
{
	class Camera : public Entity
	{
	public:
		Camera(const char* name)
			: Entity(name), m_AspectRatio(16.0f / 9.0f), m_NearPlane(0.1f), m_FarPlane(100.0f), m_FOV(60.0f)
		{
			GetTransform().SetPosition({0.0f, 2.0f, 10.0f});
			GetTransform().SetRotation(glm::quat({ 0.0f, 0.0f, 0.0f }));
			GetTransform().SetScale({ 1.0f, 1.0f, 1.0f});

			GetViewMatrix();
			GetProjectionMatrix();
		}
		
		virtual ~Camera() = default;

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

		static std::unique_ptr<Camera> Create(const char* name) { return std::make_unique<Camera>(name); }
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