#include "Engine/Framework/Transform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine::Framework
{
	void Transform::Recalculate() const
	{
		glm::mat4 position = glm::translate(glm::mat4(1.0f), m_Position);

		glm::quat rotation = 
			glm::rotate(glm::mat4(1.0f), m_Rotation.x, glm::vec3(1, 0, 0)) *
			glm::rotate(glm::mat4(1.0f), m_Rotation.x, glm::vec3(1, 0, 0)) *
			glm::rotate(glm::mat4(1.0f), m_Rotation.x, glm::vec3(1, 0, 0));

		m_WorldMatrix = position * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), m_Scale);
	}

	const glm::mat4& Transform::GetWorldMatrix() const
	{
		if (m_Dirty)
			Recalculate();

		return m_WorldMatrix;
	}

	void Transform::MarkDirty()
	{
		m_Dirty = true;
		m_ModifiedThisFrame = true;
	}
}