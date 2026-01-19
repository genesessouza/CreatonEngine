#pragma once

#include <Engine/Rendering/Buffer/VertexBufferLayout.h>

#include <glad/glad.h>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace Engine::Framework::Geometry
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;

		static Engine::Rendering::Buffer::VertexBufferLayout GetLayout()
		{
			Engine::Rendering::Buffer::VertexBufferLayout layout;

			layout.Add(0, 3, GL_FLOAT, false); // position
			layout.Add(1, 3, GL_FLOAT, false); // normal
			layout.Add(2, 2, GL_FLOAT, false); // uv

			return layout;
		}
	};

	class Mesh
	{
	public:
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		static Mesh CreateTriangle()
		{
			Mesh mesh;

			glm::vec3 normal = { 0.0f, 0.0f, 1.0f };

			mesh.vertices = {
				{{ 0.0f,  0.5f, 0.0f }, normal, {0.5f, 1.0f}},
				{{-0.5f, -0.5f, 0.0f }, normal, {0.0f, 0.0f}},
				{{ 0.5f, -0.5f, 0.0f }, normal, {1.0f, 0.0f}}
			};

			mesh.indices = {
				0, 1, 2
			};

			return mesh;
		}

		static Mesh CreateQuad()
		{
			Mesh mesh;
			mesh.vertices = {
				{{-0.5f,  0.5f, 0.0f }, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
				{{-0.5f, -0.5f, 0.0f }, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
				{{ 0.5f, -0.5f, 0.0f }, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
				{{ 0.5f,  0.5f, 0.0f }, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}
			};

			mesh.indices = {
				0, 1, 2,
				2, 3, 0
			};

			return mesh;
		}

		static Mesh CreateCube()
		{
			Mesh mesh;

			mesh.vertices = {
				// Front face (Z+) - Normal: {0, 0, 1}
				{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
				{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
				{{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
				{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},

				// Back face (Z-) - Normal: {0, 0, -1}
				{{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
				{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
				{{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
				{{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},

				// Left face (X-) - Normal: {-1, 0, 0}
				{{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
				{{-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
				{{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
				{{-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

				// Right face (X+) - Normal: {1, 0, 0}
				{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
				{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
				{{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
				{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

				// Top face (Y+) - Normal: {0, 1, 0}
				{{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
				{{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
				{{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
				{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},

				// Bottom face (Y-) - Normal: {0, -1, 0}
				{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
				{{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
				{{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
				{{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}
			};

			mesh.indices = {
				0,  1,  2,  2,  3,  0,  // Front
				4,  5,  6,  6,  7,  4,  // Back
				8,  9,  10, 10, 11, 8,  // Left
				12, 13, 14, 14, 15, 12, // Right
				16, 17, 18, 18, 19, 16, // Top
				20, 21, 22, 22, 23, 20  // Bottom
			};

			return mesh;
		}
	};
}