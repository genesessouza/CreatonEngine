#pragma once

#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace Engine::Framework
{
	class Entity;
	class Camera;
	class Scene;
	class Debugging;
	class Collider;

	class Raycast
	{
	public:
		struct RayResult 
		{
			bool Success = false;
			Entity* HitEntity = nullptr;
			
			float Distance = 0.0f;
			glm::vec3 HitPoint = glm::vec3(0.0f);
		};

		static RayResult MouseToWorldPos(const Camera& camera, bool debug);
	};
}