#pragma once

#include <glm/glm.hpp>

namespace Engine::Framework
{
	class Transform
	{
	public:
		Transform() : m_Position(0), m_Rotation(0), m_Scale(1), m_Dirty(true)
		{
		}

		virtual ~Transform() = default;

		void SetPosition(const glm::vec3& position)
		{
			if (m_Position != position)
			{
				m_Position = position;
				m_Dirty = true;
			}
		}

		void SetRotation(const glm::vec3& rotation)
		{
			if (m_Rotation != rotation)
			{
				m_Rotation = rotation;
				m_Dirty = true;
			}
		}

		void SetScale(const glm::vec3& scale)
		{
			if (m_Scale != scale)
			{
				m_Scale = scale;
				m_Dirty = true;
			}
		}

		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetRotation() const { return m_Rotation; }
		const glm::vec3& GetScale() const { return m_Scale; }

		bool IsDirty() const { return m_Dirty; }
		void ClearDirty() const { m_Dirty = false; }

		glm::mat4 GetMatrix() const;
	private:
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;

		mutable glm::mat4 m_CachedMatrix = glm::mat4(1.0f);
		mutable bool m_Dirty = false;
	};
}