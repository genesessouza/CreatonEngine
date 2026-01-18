#include "Engine/Rendering/Shader.h"

#include <Engine/Core/Log/Logger.h>
#include <Engine/Core/Paths.h>

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <sstream>
#include <fstream>

namespace Engine::Rendering
{
	std::shared_ptr<Shader> Shader::CreateDefaultShader()
	{
		//return std::make_shared<Shader>("D:/Dev/CreationEngine/Assets/Shaders/Dummy-Test.cshader");
		return std::make_shared<Shader>("D:/Dev/CreationEngine/Assets/Shaders/Default-Lit.cshader");
	}

	void Shader::GetShaderInfoLog(const std::string& uniformName, unsigned int location) const
	{
		if (location == -1)
		{
			int messageLength;
			glGetShaderiv(m_RendererId, GL_INFO_LOG_LENGTH, &messageLength);
			char* message = (char*)_malloca(messageLength * sizeof(char));

			glGetShaderInfoLog(m_RendererId, messageLength, &messageLength, message);
			CRTN_LOG_INFO("Uniform [%s] not found!: %s", uniformName.c_str(), message);
		}
	}

	Shader::Shader(const std::string& shaderFilepath)
		: m_ShaderFilepath(shaderFilepath)
	{
		ShaderSource sources = ParseShader();

		CreateShader(sources.VertexSource, sources.FragmentSource);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererId);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererId);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	ShaderSource Shader::ParseShader() const
	{
		std::ifstream stream(m_ShaderFilepath);

		std::string line;
		std::stringstream ss[2];

		ShaderType type = ShaderType::None;

		while (getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
					type = ShaderType::Vertex;
				else if (line.find("fragment") != std::string::npos)
					type = ShaderType::Fragment;
			}
			else
			{
				ss[(int)type] << line << "\n";
			}
		}
		return { ss[0].str(), ss[1].str() };
	}

	uint32_t Shader::CompileShader(unsigned int type, const std::string& shaderSource)
	{
		unsigned int shaderId = glCreateShader(type);
		const char* source = shaderSource.c_str();

		glShaderSource(shaderId, 1, &source, nullptr);
		glCompileShader(shaderId);

		int compilationResult;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compilationResult);

		if (compilationResult == GL_FALSE)
		{
			int messageLength;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &messageLength);
			char* message = (char*)_malloca(messageLength * sizeof(char));

			glGetShaderInfoLog(shaderId, messageLength, &messageLength, message);

			const char* shaderType;
			if (type == GL_VERTEX_SHADER)
				shaderType = "Vertex Shader";
			else
				shaderType = "Fragment Shader";

			glDeleteShader(shaderId);
			CRTN_ASSERT(false, "Failed to compile %s!", shaderType);
			CRTN_LOG_ERROR("%s", message);

			return 0;
		}

		return shaderId;
	}

	void Shader::CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
	{
		m_RendererId = glCreateProgram();

		unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
		unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

		glAttachShader(m_RendererId, vertexShader);
		glAttachShader(m_RendererId, fragmentShader);

		glLinkProgram(m_RendererId);

		uint32_t isLinked = 0;
		glGetProgramiv(m_RendererId, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			int messageLength = 0;
			glGetProgramiv(m_RendererId, GL_INFO_LOG_LENGTH, &messageLength);

			char* message = (char*)_malloca(messageLength * sizeof(char));
			glGetProgramInfoLog(m_RendererId, messageLength, &messageLength, &message[0]);

			glDeleteProgram(m_RendererId);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			CRTN_LOG_ERROR("%s", message);
			CRTN_ASSERT(false, "Shader link compilation failed!");

			return;
		}
		glValidateProgram(m_RendererId);

		glDetachShader(m_RendererId, vertexShader);
		glDetachShader(m_RendererId, fragmentShader);
	}

	void Shader::DefineUniformMat4(const std::string& uniformName, const glm::mat4& matrix) const
	{
		unsigned int location = glGetUniformLocation(m_RendererId, uniformName.c_str());
		glUseProgram(m_RendererId);

		GetShaderInfoLog(uniformName, location);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::DefineUniformVec3(const std::string& uniformName, const glm::vec3& vector) const
	{
		unsigned int location = glGetUniformLocation(m_RendererId, uniformName.c_str());
		glUseProgram(m_RendererId);

		GetShaderInfoLog(uniformName, location);
		glUniform3fv(location, 1, glm::value_ptr(vector));
	}

	void Shader::DefineUniformVec4(const std::string& uniformName, const glm::vec4& matrix) const
	{
		unsigned int location = glGetUniformLocation(m_RendererId, uniformName.c_str());
		glUseProgram(m_RendererId);

		GetShaderInfoLog(uniformName, location);
		glUniform4fv(location, 1, glm::value_ptr(matrix));
	}

	void Shader::DefineUniformBool(const std::string& uniformName, bool value) const
	{
		unsigned int location = glGetUniformLocation(m_RendererId, uniformName.c_str());
		glUseProgram(m_RendererId);

		if (location != -1)
			glUniform1i(location, (int)value); // OpenGL trata bool como int
	}

	const glm::vec3 Shader::GetUniformVec3(const std::string& uniformName) const
	{
		unsigned int location = glGetUniformLocation(m_RendererId, uniformName.c_str());
		glUseProgram(m_RendererId);

		GetShaderInfoLog(uniformName, location);

		float vector[3];
		glGetUniformfv(location, 1, vector);

		return glm::vec3(vector[0], vector[1], vector[2]);
	}

	const glm::vec4 Shader::GetUniformVec4(const std::string& uniformName) const
	{
		unsigned int location = glGetUniformLocation(m_RendererId, uniformName.c_str());
		glUseProgram(m_RendererId);

		GetShaderInfoLog(uniformName, location);

		float vector[4];
		glGetUniformfv(location, 1, vector);

		return glm::vec4(vector[0], vector[1], vector[2], vector[3]);
	}

	const glm::mat4 Shader::GetUniformMat4(const std::string& uniformName) const
	{
		unsigned int location = glGetUniformLocation(m_RendererId, uniformName.c_str());
		glUseProgram(m_RendererId);

		GetShaderInfoLog(uniformName, location);

		float matrix[16];
		glGetUniformfv(location, 1, matrix);

		return glm::mat4(glm::make_mat4(matrix));
	}
}