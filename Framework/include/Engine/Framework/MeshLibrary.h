#pragma once

#include "Engine/Framework/Mesh.h"

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

		static Engine::Framework::Geometry::Mesh GetCPUMeshQuad() {
			return Geometry::Mesh::CreateQuad();
		}

		static Engine::Framework::Geometry::Mesh GetCPUMeshCube() {
			return Geometry::Mesh::CreateCube();
		}

	private:
		static MeshGPURef CreateQuad()
		{
			Engine::Framework::Geometry::Mesh cpuMesh = Geometry::Mesh::CreateQuad();
			return std::make_shared<Engine::Rendering::MeshGPU>(cpuMesh);
		}

		static MeshGPURef CreateCube()
		{
			Engine::Framework::Geometry::Mesh cpuMesh = Geometry::Mesh::CreateCube();
			return std::make_shared<Engine::Rendering::MeshGPU>(cpuMesh);
		}
	};
}