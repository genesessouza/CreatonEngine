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
		Normal,
		Color,
		Intensity,
		Diffuse,
		Specular,
		Shininess
	};

	class Shader
	{
	public:
		struct SceneUniformLocations
		{
			int ViewProjection;

			int ViewPos;
			int Normal;

			int HasDirLight;
			int DirLightDir;
			int DirLightColor;
			int DirLightIntensity;

			int PointLightCount;
			int PointLightPos[8];
			int PointLightColor[8];
			int PointLightIntensity[8];
		};
	public:
		SceneUniformLocations sceneUniforms;
	public:
		Shader(const std::string& shaderFilepath);
		~Shader();

		void Bind() const;
		void Unbind() const;

		int GetUniformLocation(const std::string& name);
		
		void DefineUniformMat3(int loc, const glm::mat3& matrix);
		void DefineUniformMat4(int loc, const glm::mat4& matrix);

		void DefineUniformVec3(int loc, const glm::vec3& vector);
		void DefineUniformVec4(int loc, const glm::vec4& vector);

		void DefineUniformBool(int loc, const bool override);

		void DefineUniformInt(int loc, const int value);
		void DefineUniformFloat(int loc, const float value);
		void DefineUniformFloat3(int loc, const glm::vec3& value);

		const glm::vec3& GetUniformVec3(const std::string& uniformName);
		const glm::vec4& GetUniformVec4(const std::string& uniformName);

		const glm::mat3& GetUniformMat3(const std::string& uniformName);
		const glm::mat4& GetUniformMat4(const std::string& uniformName);
		
		const int GetUniformInt(const std::string& uniformName);

		const float GetUniformFloat(const std::string& uniformName);

		const bool GetUniformBool(const std::string& uniformName);

		static std::shared_ptr<Shader> CreateLitShader(); // Blinn Phong + Shadows
		static std::shared_ptr<Shader> CreateUnlitShader(); // Ambient only
		static std::shared_ptr<Shader> CreateBillboardShader(); // Textures only. For camera, lights, particles
		static std::shared_ptr<Shader> CreateWireframeShader(); // Collider Rendering
		static std::shared_ptr<Shader> CreateDebugShader(); // View Modes: Normals, Diffuse, Specular, Polygon

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
				case Normal:
					return "u_NormalMatrix";
					break;
				case Color:
					return "u_Color";
					break;
				case Intensity:
					return "u_LightIntensity";
					break;
				case Diffuse:
					return "u_DiffuseStrength";
					break;
				case Specular:
					return "u_SpecularStrength";
					break;
				case Shininess:
					return "u_Shininess";
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
	private:
		std::unordered_map<std::string, int> m_UniformLocationCache;
	};
}