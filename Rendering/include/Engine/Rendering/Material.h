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
			m_Shader->Bind();
			m_Shader->DefineUniformVec4(m_Shader->GetUniformLocation(m_Shader->GetDefaultUniformNames(UniformType::Color)), m_Color);
			m_Shader->DefineUniformFloat(m_Shader->GetUniformLocation(m_Shader->GetDefaultUniformNames(UniformType::Diffuse)), m_Diffuse);
			m_Shader->DefineUniformFloat(m_Shader->GetUniformLocation(m_Shader->GetDefaultUniformNames(UniformType::Specular)), m_Specular);
			m_Shader->DefineUniformFloat(m_Shader->GetUniformLocation(m_Shader->GetDefaultUniformNames(UniformType::Shininess)), m_Shininess);
		}

		void SetColor(const glm::vec4& color) { m_Color = color; }
		const glm::vec4& GetColor() const { return m_Color; }

		void SetDiffuse(float value) { m_Diffuse = glm::clamp(value, 0.0f, 1.0f); }
		float GetDiffuse() const { return m_Diffuse; }

		void SetSpecular(float value) { m_Specular = glm::clamp(value, 0.0f, 1.0f); }
		float GetSpecular() const { return m_Specular; }

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
		std::shared_ptr<Shader> m_Shader;

		glm::vec4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float m_Diffuse = 0.5f;
		float m_Specular = 0.5f;
		float m_Shininess = 32.0f;
	};
}