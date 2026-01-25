#pragma once

#include <glm/vec3.hpp>

namespace Engine::Framework::Physics
{
	class Gravity
	{
	public:
		static const glm::vec3& GravityAcceleration()
		{
			static const glm::vec3 gravityVector(0.0f, -9.81f, 0.0f);
			return gravityVector;
		}
	};
}