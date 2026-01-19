#pragma once

#include "Engine/Rendering/Shader.h"

#include <glm/ext/vector_float3.hpp>

namespace Engine::Rendering
{
	class Material
	{
	public:
		Material()
			: m_Shader(nullptr)
		{
		}

		virtual ~Material() = default;

		void Init()
		{
			m_Shader = Engine::Rendering::Shader::CreateDefaultShader();
			CRTN_CHECK_PTR(m_Shader);
		}

		inline void Bind() const 
		{
			CRTN_CHECK_PTR(m_Shader);
			m_Shader->Bind(); 
		}

		inline void SetModelMatrix(glm::mat4 transformMatrix)
		{
			//CRTN_CHECK_PTR(m_Shader);
			m_Shader->DefineUniformMat4(m_Shader->GetDefaultUniformNames(UniformType::Model), transformMatrix);
		}

		inline void SetViewProjectionMatrix(glm::mat4 cameraViewProjectionMatrix)
		{
			//CRTN_CHECK_PTR(m_Shader);
			m_Shader->DefineUniformMat4(m_Shader->GetDefaultUniformNames(UniformType::ViewProjection), cameraViewProjectionMatrix);
		}

		void SetColor(glm::vec4 color) 
		{
			CRTN_CHECK_PTR(m_Shader);
			m_Shader->DefineUniformVec4("u_Color", glm::vec4(color.r, color.g, color.b, color.a)); 
		}

		const std::shared_ptr<Shader> GetShader()
		{
			CRTN_CHECK_PTR(m_Shader);
			return m_Shader;
		}

		static std::shared_ptr<Material> Create()
		{
			auto mat = std::make_shared<Material>();
			CRTN_CHECK_PTR(mat);

			return mat;
		}
	private:
		std::shared_ptr<Shader> m_Shader;
	};
}