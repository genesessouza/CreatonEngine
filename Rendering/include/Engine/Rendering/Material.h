#pragma once

#include "Engine/Rendering/Shader.h"

#include <glm/ext/vector_float3.hpp>

namespace Engine::Rendering
{
	class Material
	{
	public:
		static std::shared_ptr<Material> Create() { return std::make_shared<Material>(); }

		void SetColor(glm::vec4 color) { m_Shader->DefineUniformVec4("u_Color", glm::vec4(color.r, color.g, color.b, color.a)); }
	private:
		std::shared_ptr<Shader> m_Shader;
	};
}