#pragma once

#include "Engine/Framework/Transform.h"

#include <string>

namespace Engine::Framework
{
	class Entity
	{
	public:
		Entity(const char* name = "Entity") : m_Name(name), m_IsEnabled(true) {}
		virtual ~Entity() = default;

		virtual void OnUpdate() {}

		void SetEnabled(bool enabled) { m_IsEnabled = enabled; }
		bool IsEnabled() const { return m_IsEnabled; }

		const std::string& GetName() const { return m_Name; }
		void SetName(const std::string& name) { m_Name = name; }

		Transform& GetTransform() { return m_Transform; }
		const Transform& GetTransform() const { return m_Transform; }
	protected:
		std::string m_Name;
		bool m_IsEnabled;

		Transform m_Transform;
	};
}