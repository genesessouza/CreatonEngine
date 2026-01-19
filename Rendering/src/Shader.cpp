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
		//auto shader = std::make_shared<Shader>("D:/Dev/CreationEngine/_Assets/Shaders/Dummy-Test.cshader");
		auto shader = std::make_shared<Shader>("D:/Dev/CreationEngine/_Assets/Shaders/Default-Lit.cshader");
		CRTN_CHECK_PTR(shader);

		return shader;
	}

	void Shader::GetShaderInfoLog(const std::string& uniformName, unsigned int location) const
	{
		if (location == -1)
		{
			int messageLength;
			glGetShaderiv(m_RendererId, GL_INFO_LOG_LENGTH, &messageLength);
			char* message = (char*)_malloca(messageLength * sizeof(char));

			glGetShaderInfoLog(m_RendererId, messageLength, &messageLength, message);
			CRTN_LOG_ERROR("[SHADER]: Uniform [%s] not found!: %s", uniformName.c_str(), message);
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
		if (!stream.is_open()) {
			CRTN_ASSERT(false, "Could not open shader file: %s", m_ShaderFilepath.c_str());
			return { "", "" };
		}

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::None;

		bool foundVertex = false;
		bool foundFragment = false;

		while (getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos) {
					type = ShaderType::Vertex;
					foundVertex = true;
				}
				else if (line.find("fragment") != std::string::npos) {
					type = ShaderType::Fragment;
					foundFragment = true;
				}
			}
			else
			{
				if (type != ShaderType::None)
					ss[(int)type] << line << "\n";
			}
		}

		if (!foundVertex || !foundFragment) {
			CRTN_ASSERT(false, "Parsing error: %s! Tag %s not found!", m_ShaderFilepath.c_str(), (!foundVertex ? "#vertex" : "#fragment"));
		}

		return { ss[0].str(), ss[1].str() };
	}

	uint32_t Shader::CompileShader(unsigned int type, const std::string& shaderSource)
	{
		unsigned int shaderId = glCreateShader(type);
		glObjectLabel(GL_SHADER, shaderId, -1, m_ShaderFilepath.c_str());

		const char* source = shaderSource.c_str();

		glShaderSource(shaderId, 1, &source, nullptr);
		glCompileShader(shaderId);

		int compilationResult;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compilationResult);

		if (compilationResult == GL_FALSE)
		{
			int messageLength;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &messageLength);

			std::vector<char> message(messageLength + 1);
			glGetShaderInfoLog(shaderId, messageLength, &messageLength, message.data());

			const char* shaderType = (type == GL_VERTEX_SHADER) ? "Vertex Shader" : "Fragment Shader";
			
			CRTN_LOG_ERROR("Compilation failed: File: %s - %s:\n%s", m_ShaderFilepath.c_str(), (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"), message.data());	
			glDeleteShader(shaderId);

			CRTN_ASSERT(false, "Shader compilation failed!");

			return 0;
		}

		return shaderId;
	}

	void Shader::CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
	{
		m_RendererId = glCreateProgram();

		unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
		unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

		if (vertexShader == 0 || fragmentShader == 0) return;

		glAttachShader(m_RendererId, vertexShader);
		glAttachShader(m_RendererId, fragmentShader);

		glLinkProgram(m_RendererId);

		int isLinked = 0;
		glGetProgramiv(m_RendererId, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			int messageLength = 0;
			glGetProgramiv(m_RendererId, GL_INFO_LOG_LENGTH, &messageLength);

			std::vector<char> message(messageLength + 1);
			glGetProgramInfoLog(m_RendererId, messageLength, &messageLength, message.data());

			CRTN_LOG_ERROR("Linking error: File: %s:\n%s", m_ShaderFilepath.c_str(), message.data());

			glDeleteProgram(m_RendererId);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			CRTN_ASSERT(false, "Shader linking failed!");
			return;
		}
		glValidateProgram(m_RendererId);

		glDetachShader(m_RendererId, vertexShader);
		glDetachShader(m_RendererId, fragmentShader);
	}

	void Shader::DefineUniformMat3(const std::string& uniformName, const glm::mat3& matrix) const
	{
		unsigned int location = glGetUniformLocation(m_RendererId, uniformName.c_str());
		glUseProgram(m_RendererId);

		GetShaderInfoLog(uniformName, location);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
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

		GetShaderInfoLog(uniformName, location);

		if (location != -1)
			glUniform1i(location, (int)value); // OpenGL treats bool as int
	}

	void Shader::DefineUniformInt(const std::string& uniformName, const int value) const
	{
		unsigned int location = glGetUniformLocation(m_RendererId, uniformName.c_str());
		glUseProgram(m_RendererId);

		GetShaderInfoLog(uniformName, location);

		if (location != -1)
			glUniform1i(location, value);
	}

	void Shader::DefineUniformFloat(const std::string& uniformName, const float value) const
	{
		unsigned int location = glGetUniformLocation(m_RendererId, uniformName.c_str());
		glUseProgram(m_RendererId);

		GetShaderInfoLog(uniformName, location);

		if (location != -1)
			glUniform1f(location, value);
	}

	void Shader::DefineUniformFloat3(const std::string& uniformName, const glm::vec3& value) const
	{
		unsigned int location = glGetUniformLocation(m_RendererId, uniformName.c_str());
		glUseProgram(m_RendererId);

		GetShaderInfoLog(uniformName, location);

		if (location != -1)
			glUniform3f(location, value.x, value.y, value.z);
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