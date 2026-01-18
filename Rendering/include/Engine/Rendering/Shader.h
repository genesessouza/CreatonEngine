#pragma once

#include <glm/glm.hpp>

#include <cstdint>
#include <string>
#include <memory>

namespace Engine::Rendering
{
	struct ShaderSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	enum UniformType
	{
		Model,
		View,
		Projection,
		ViewProjection,
		Transform,
		Color,
		ColorOverride
	};

	class Shader
	{
	public:
		Shader(const std::string& shaderFilepath);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void GetShaderInfoLog(const std::string& uniformName, unsigned int location) const;

		void DefineUniformMat4(const std::string& uniformName, const glm::mat4& matrix) const;

		void DefineUniformVec3(const std::string& uniformName, const glm::vec3& vector) const;
		void DefineUniformVec4(const std::string& uniformName, const glm::vec4& vector) const;

		void DefineUniformBool(const std::string& uniformName, const bool override) const;

		const glm::vec3 GetUniformVec3(const std::string& uniformName) const;
		const glm::vec4 GetUniformVec4(const std::string& uniformName) const;
		const glm::mat4 GetUniformMat4(const std::string& uniformName) const;

		static std::shared_ptr<Shader> CreateDefaultShader();
	public:
		inline uint32_t GetShader() const { return m_RendererId; }

		std::string GetDefaultUniformNames(UniformType uniform)
		{
			switch (uniform)
			{
			case Model:
				return "u_ModelMatrix";
				break;
			case View:
				return "u_ViewMatrix";
				break;
			case Projection:
				return "u_ProjectionMatrix";
				break;
			case ViewProjection:
				return "u_ViewProjectionMatrix";
				break;
			case Transform:
				return "u_Transform";
				break;
			case Color:
				return "u_Color";
				break;
			default:
				return "Invalid uniform name!";
				break;
			}
		}
	private:
		enum class ShaderType { None = -1, Vertex = 0, Fragment = 1 };
	private:
		ShaderSource ParseShader() const;
		uint32_t CompileShader(unsigned int type, const std::string& shaderSource);
		void CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
	private:
		std::string m_ShaderFilepath;

		uint32_t m_RendererId;
	};
}