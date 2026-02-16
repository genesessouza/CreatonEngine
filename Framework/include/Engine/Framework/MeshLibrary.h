#pragma once

#include <Engine/Rendering/MeshGPU.h>

namespace Engine::Framework
{
	class MeshLibrary
	{
	public:
		using MeshGPURef = std::shared_ptr<Engine::Rendering::MeshGPU>;
	public:
		static MeshGPURef InstantiateQuad()
		{
			static MeshGPURef quadMesh = CreateQuad();
			return quadMesh;
		}

		static MeshGPURef InstantiateCube()
		{
			static MeshGPURef cubeMesh = CreateCube();
			return cubeMesh;
		}

		static MeshGPURef InstantiateSphere()
		{
			static MeshGPURef sphereMesh = CreateSphere();
			return sphereMesh;
		}

		static Geometry::Mesh GetCPUMeshQuad() {
			return Geometry::Mesh::CreateQuad();
		}

		static Geometry::Mesh GetCPUMeshCube() {
			return Geometry::Mesh::CreateCube();
		}

		static Geometry::Mesh GetCPUMeshSphere() {
			return Geometry::Mesh::CreateSphere();
		}

	private:
		static MeshGPURef CreateQuad()
		{
			Geometry::Mesh cpuMesh = Geometry::Mesh::CreateQuad();
			return std::make_shared<Engine::Rendering::MeshGPU>(cpuMesh);
		}

		static MeshGPURef CreateCube()
		{
			Geometry::Mesh cpuMesh = Geometry::Mesh::CreateCube();
			return std::make_shared<Engine::Rendering::MeshGPU>(cpuMesh);
		}

		static MeshGPURef CreateSphere()
		{
			Geometry::Mesh cpuMesh = Geometry::Mesh::CreateSphere();
			return std::make_shared<Engine::Rendering::MeshGPU>(cpuMesh);
		}
	};
}