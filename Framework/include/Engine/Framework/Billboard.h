#pragma once

#include "Engine/Framework/Component.h"
#include "Engine/Framework/Transform.h"

#include <glm/vec4.hpp>
#include <memory>

namespace Engine::Rendering
{
	class Shader;

	namespace Array { class VertexArray; }
	namespace Buffer { class VertexBuffer; class IndexBuffer; }
}
namespace Engine::Framework
{
	class Camera;
	class Entity;

	class Billboard : public Component
	{
	public:
		struct BillboardUniformLocations
		{
			int View = -1;
			int ViewProjection = -1;
			int Position = -1;
			int Color = -1;
			int Size = -1;
		};
	public:
		Billboard() : Component(), m_Size(1.0f), m_Color(glm::vec4(1.0f)) {}
		virtual ~Billboard() = default;

		void Init();

		void Draw(const Transform& transform, const Camera& sceneCamera);

		void SetSize(float size) { m_Size = size; }
		void SetColor(const glm::vec4& color) { m_Color = color; }

		float GetSize() const { return m_Size; }
		glm::vec4 GetColor() const { return m_Color; }
	private:
		void InitUniforms();
	private:
		float m_Size = 1.0f;
		glm::vec4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };

		BillboardUniformLocations m_BillboardUniforms;
		bool m_UniformsInitialized = false;
	private:
		std::shared_ptr<Engine::Rendering::Shader> m_Shader;

		std::shared_ptr<Engine::Rendering::Array::VertexArray> m_VAO;
		std::shared_ptr<Engine::Rendering::Buffer::VertexBuffer> m_VBO;
		std::shared_ptr<Engine::Rendering::Buffer::IndexBuffer> m_IBO;
	};
}