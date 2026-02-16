#pragma once

#include "Engine/Rendering/Shader.h"
#include "Engine/Framework/Entity.h"

#include <glm/ext/vector_float3.hpp>

#include <Engine/Core/Log/Logger.h>

namespace Engine::Rendering
{
	class Material
	{
	public:
		Material()
			: m_Shader(nullptr), m_ColorLoc(-1), m_DiffuseLoc(-1), m_SpecularLoc(-1), m_ShininessLoc(-1)
		{
		}

		virtual ~Material() = default;

		inline void Init()
		{
			m_Shader = Shader::CreateLitShader();
			CRTN_CHECK_PTR(m_Shader);

			CacheUniforms();
		}

		inline void Bind() const
		{
			m_Shader->Bind();

			m_Shader->DefineUniformVec4(m_ColorLoc, m_Color);
			m_Shader->DefineUniformFloat(m_DiffuseLoc, m_Diffuse);
			m_Shader->DefineUniformFloat(m_SpecularLoc, m_Specular);
			m_Shader->DefineUniformFloat(m_ShininessLoc, m_Shininess);
		}

		void SetColor(const glm::vec4& color) { m_Color = color; }
		const glm::vec4& GetColor() const { return m_Color; }

		void SetDiffuse(float value) { m_Diffuse = glm::clamp(value, 0.0f, 1.0f); }
		float GetDiffuse() const { return m_Diffuse; }

		void SetSpecular(float value) { m_Specular = glm::clamp(value, 0.0f, 1.0f); }
		float GetSpecular() const { return m_Specular; }

		void SetSpecularColor(const glm::vec4& color) { m_SpecularColor = color; }
		const glm::vec4& GetSpecularColor() const { return m_SpecularColor; }

		void SetShininess(float value) { m_Shininess = glm::clamp(value, 8.0f, 128.0f); }
		float GetShininess() const { return m_Shininess; }

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
		void CacheUniforms()
		{
			m_ColorLoc = m_Shader->GetUniformLocation("u_Color");
			m_DiffuseLoc = m_Shader->GetUniformLocation("u_DiffuseStrength");
			m_SpecularLoc = m_Shader->GetUniformLocation("u_SpecularStrength");
			m_SpecularColorLoc = m_Shader->GetUniformLocation("u_SpecularColor");
			m_LightSpaceMatrixLoc = m_Shader->GetUniformLocation("u_LightSpaceMatrix");
			m_ShadowMapTextureLoc = m_Shader->GetUniformLocation("u_ShadowMap");
			m_ShininessLoc = m_Shader->GetUniformLocation("u_Shininess");
		}

		int m_ColorLoc, m_DiffuseLoc, m_SpecularLoc, m_SpecularColorLoc, m_ShininessLoc, m_LightSpaceMatrixLoc, m_ShadowMapTextureLoc;
	private:
		std::shared_ptr<Shader> m_Shader;

		glm::vec4 m_Color{ 1.0f };
		float m_Diffuse = 0.5f;

		float m_Specular = 0.5f;
		glm::vec4 m_SpecularColor{ 1.0f };
		float m_Shininess = 32.0f;
	};
}