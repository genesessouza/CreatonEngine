#pragma once

#include "Engine/Rendering/Shader.h"
#include "Engine/Rendering/Renderer.h"

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

		inline void Init()
		{
			m_Shader = Engine::Rendering::Shader::CreateDefaultShader();
			CRTN_CHECK_PTR(m_Shader);

			Engine::Rendering::Renderer::InitSceneUniforms(m_Shader);
		}

		inline void Bind() const
		{
			CRTN_CHECK_PTR(m_Shader);
			m_Shader->Bind();
		}

		inline void SetColor(const glm::vec4& color, const float intensity)
		{
			CRTN_CHECK_PTR(m_Shader);
			
			m_Shader->DefineUniformVec4(
				m_Shader->GetUniformLocation(m_Shader->GetDefaultUniformNames(UniformType::Color)), 
				glm::vec4(color.r, color.g, color.b, color.a) * intensity
			);
		}

		/// <summary>
		/// Specular strength. Clamped between 0 - 1
		/// </summary>
		/// <param name="value"></param>
		inline void SetSpecular(float value)
		{
			CRTN_CHECK_PTR(m_Shader);
			
			if (value < 0)
				value = 0;
			else if (value > 1)
				value = 1;
			else
				m_Shader->DefineUniformFloat(m_Shader->GetUniformLocation("u_SpecularStrength"), value);
		}

		/// <summary>
		/// Defines how sharp should specular reflection be.
		/// Clamped between 8 and 128.
		/// Incremented by multipliying or dividind by 2 fold
		/// </summary>
		/// <param name="value"></param>
		inline void SetShininess(float value)
		{
			CRTN_CHECK_PTR(m_Shader);

			if (value < 8)
				value = 8;
			else if (value > 128)
				value = 128;
			else
				m_Shader->DefineUniformFloat(m_Shader->GetUniformLocation("u_Shininess"), value);
		}

		const std::shared_ptr<Shader>& GetShaderID() const
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