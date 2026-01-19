#include "Engine/Framework/Camera.h"

#include <Engine/Core/Log/Logger.h>

#include <glm/ext/matrix_transform.hpp>

namespace Engine::Framework
{
	Camera::Camera()
	{
		m_AspectRatio = 16.0f / 9.0f;

		GetViewMatrix();
		GetProjectionMatrix();
	}
	const glm::mat4 Camera::GetViewMatrix()
	{
		if (m_Dirty)
		{
			m_CachedViewMatrix = glm::inverse(m_Transform.GetMatrix());
			m_Dirty = false;
		}
		return m_CachedViewMatrix;
	}

	const glm::mat4 Camera::GetProjectionMatrix()
	{
		m_ProjectionMatrix = glm::perspective(
			glm::radians(m_FOV),
			m_AspectRatio,
			m_NearPlane,
			m_FarPlane
		);

		return m_ProjectionMatrix;
	}

	const glm::mat4 Camera::GetViewProjectionMatrix() const
	{
		return m_ProjectionMatrix * m_CachedViewMatrix;
	}

	void Camera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;

		m_ProjectionMatrix = glm::perspective(
			glm::radians(m_FOV),
			m_AspectRatio,
			m_NearPlane,
			m_FarPlane
		);

		m_Dirty = true;
	}

	const void Camera::MoveCamera(const glm::vec3& position)
	{
		glm::vec3 newPosition = m_Transform.GetPosition() + glm::vec3(position.x, position.y, -position.z);
		m_Transform.SetPosition(newPosition);
		m_Dirty = true;

		GetViewMatrix();
	}

	const void Camera::RotateCamera(const glm::vec3& rotation)
	{
		glm::vec3 newRotation = m_Transform.GetRotation() + glm::vec3(-rotation.x, -rotation.y, rotation.z);
		m_Transform.SetRotation(newRotation);
		m_Dirty = true;

		GetViewMatrix();
	}
}