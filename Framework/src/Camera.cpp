#include "Engine/Framework/Camera.h"

#include "Engine/Framework/Entity.h"
#include "Engine/Framework/Transform.h"

#include <Engine/Core/Log/Logger.h>

namespace Engine::Framework
{
	void Camera::Init()
	{
		m_Owner->GetTransform().SetPosition({ 0.0f, 2.0f, 10.0f });
		m_Owner->GetTransform().SetRotation(glm::vec3(0.0f));
		m_Owner->GetTransform().SetScale({ 1.0f, 1.0f, 1.0f });

		GetViewMatrix();
		GetProjectionMatrix();
	}

	const glm::mat4& Camera::GetViewMatrix() const
	{
		m_CachedViewMatrix = glm::inverse(m_Owner->GetTransform().GetWorldMatrix());
		return m_CachedViewMatrix;
	}

	const glm::mat4& Camera::GetProjectionMatrix() const
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
		return m_ProjectionMatrix;
	}

	glm::mat4 Camera::GetViewProjectionMatrix() const
	{
		return m_ProjectionMatrix * m_CachedViewMatrix;
	}

	void Camera::SetViewportSize(uint32_t width, uint32_t height)
	{
		if (width > 0 && height > 0)
		{
			m_AspectRatio = (float)width / (float)height;
			GetProjectionMatrix();
		}
		GetViewProjectionMatrix();
	}
}