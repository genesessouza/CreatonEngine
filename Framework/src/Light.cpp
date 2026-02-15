#include "Engine/Framework/Light.h"

#include "Engine/Framework/Scene.h"
#include "Engine/Framework/Entity.h"

namespace Engine::Framework::Lights
{
	void DirectionalLight::Init()
	{
		m_Color = glm::vec4(1.00f, 0.95f, 0.70f, 1.0f); // light yellow
		m_Intensity = 3.0f;
		m_Direction = glm::vec3(0.0f, 90.0f, 0.0f);

		GetOwner()->GetTransform().SetPosition(glm::vec3(1.0f, 5.0f, -3.0f));
		GetOwner()->GetTransform().SetScale(glm::vec3(1.0f));
	}

	void PointLight::Init()
	{
		m_Color = glm::vec4(1.0f);
		m_Intensity = 10.0f;

		GetOwner()->GetTransform().SetPosition({ 0.0f, 3.0f, 0.0f });
		GetOwner()->GetTransform().SetScale(glm::vec3(1.0f));
	}
}