#pragma once

#include <Engine/Core/Log/Logger.h>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/constants.hpp>

namespace Engine::Framework::Geometry
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;	
	};

	class Mesh
	{
	public:
		enum MeshType { NONE, TRIANGLE, QUAD, CUBE, SPHERE };
	public:
		static Mesh CreateTriangle()
		{
			Mesh mesh;

			glm::vec3 normal = { 0.0f, 0.0f, 1.0f };

			mesh.m_Vertices = {
				{{ 0.0f,  0.5f, 0.0f }, normal, {0.5f, 1.0f}},
				{{-0.5f, -0.5f, 0.0f }, normal, {0.0f, 0.0f}},
				{{ 0.5f, -0.5f, 0.0f }, normal, {1.0f, 0.0f}}
			};

			mesh.m_Indices = {
				0, 1, 2
			};

			mesh.m_MeshType = TRIANGLE;
			return mesh;
		}

		static Mesh CreateQuad()
		{
			Mesh mesh;
			mesh.m_Vertices = {
				{{-0.5f,  0.5f, 0.0f }, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
				{{-0.5f, -0.5f, 0.0f }, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
				{{ 0.5f, -0.5f, 0.0f }, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
				{{ 0.5f,  0.5f, 0.0f }, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}
			};

			mesh.m_Indices = {
				0, 1, 2,
				2, 3, 0
			};

			mesh.m_MeshType = QUAD;
			return mesh;
		}

		static Mesh CreateCube()
		{
			Mesh mesh;

			mesh.m_Vertices = {
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

			mesh.m_Indices = {
				0,  1,  2,  2,  3,  0,  // Front
				4,  5,  6,  6,  7,  4,  // Back
				8,  9,  10, 10, 11, 8,  // Left
				12, 13, 14, 14, 15, 12, // Right
				16, 17, 18, 18, 19, 16, // Top
				20, 21, 22, 22, 23, 20  // Bottom
			};

			mesh.m_MeshType = CUBE;
			return mesh;
		}

		static Mesh CreateSphere(float radius = 0.5f, uint32_t segments = 32, uint32_t rings = 16)
		{
			Mesh mesh;

			for (uint32_t y = 0; y <= rings; ++y)
			{
				for (uint32_t x = 0; x <= segments; ++x)
				{
					float xSegment = (float)x / (float)segments;
					float ySegment = (float)y / (float)rings;

					float xPos = std::cos(xSegment * 2.0f * glm::pi<float>()) * std::sin(ySegment * glm::pi<float>());
					float yPos = std::cos(ySegment * glm::pi<float>());
					float zPos = std::sin(xSegment * 2.0f * glm::pi<float>()) * std::sin(ySegment * glm::pi<float>());

					glm::vec3 pos = { xPos * radius, yPos * radius, zPos * radius };
					glm::vec3 normal = glm::normalize(pos);
					glm::vec2 uv = { xSegment, ySegment };

					mesh.m_Vertices.push_back({ pos, normal, uv });
				}
			}

			for (uint32_t y = 0; y < rings; ++y)
			{
				for (uint32_t x = 0; x < segments; ++x)
				{
					uint32_t first = (y * (segments + 1)) + x;
					uint32_t second = first + segments + 1;

					mesh.m_Indices.push_back(first);
					mesh.m_Indices.push_back(first + 1);
					mesh.m_Indices.push_back(second);

					mesh.m_Indices.push_back(second);
					mesh.m_Indices.push_back(first + 1);
					mesh.m_Indices.push_back(second + 1);
				}
			}

			mesh.m_MeshType = SPHERE;
			return mesh;
		}
	public:
		const std::vector<Vertex>& GetVertices() const { return m_Vertices; }
		const std::vector<uint32_t>& GetIndices() const { return m_Indices; }
		MeshType GetMeshType() const { return m_MeshType; }

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;

		MeshType m_MeshType = NONE;
	public:
		const char* MeshTypeToString()
		{
			switch (m_MeshType)
			{
				case NONE:     return "NONE";
				case TRIANGLE: return "TRIANGLE";
				case QUAD:     return "QUAD";
				case CUBE:     return "CUBE";
				case SPHERE:   return "SPHERE";
				default:       return "UNKNOWN";
			}
		}
	};
}