#include "Engine/Framework/Camera.h"

#include <Engine/Core/Log/Logger.h>

#include <glm/ext/matrix_transform.hpp>

namespace Engine::Framework
{
	Camera::Camera() 
		: m_AspectRatio(16.0f / 9.0f), m_NearPlane(0.1f), m_FarPlane(100.0f), m_FOV(60.0f)
	{
		GetViewMatrix();
		GetProjectionMatrix();
	}

	const glm::mat4 Camera::GetViewMatrix()
	{
		if (m_Transform.IsDirty())
		{
			m_CachedViewMatrix = glm::inverse(m_Transform.GetMatrix());
			m_Transform.ClearDirty();
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
		if (width > 0 && height > 0)
		{
			m_AspectRatio = (float)width / (float)height;

			m_ProjectionMatrix = glm::perspective(
				glm::radians(m_FOV),
				m_AspectRatio,
				m_NearPlane,
				m_FarPlane
			);
		}
		GetViewProjectionMatrix();
	}
}