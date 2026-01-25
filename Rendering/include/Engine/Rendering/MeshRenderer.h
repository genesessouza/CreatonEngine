#pragma once

#include "Engine/Rendering/MeshGPU.h"
#include "Engine/Rendering/Shader.h"
#include "Engine/Rendering/Material.h"

#include <Engine/Framework/Transform.h>
#include <Engine/Framework/Camera.h>
#include <Engine/Framework/MeshLibrary.h>

#include <memory>

namespace Engine::Rendering
{
	class MeshRenderer
	{
	public:
		struct MeshUniformLocations
		{
			int Model;
			int Normal;
		};

		MeshRenderer() : m_MeshMat(nullptr), m_MeshUniforms()
		{
		}

		virtual ~MeshRenderer() = default;

		void Init()
		{
			m_MeshMat = Engine::Rendering::Material::Create();

			if (m_MeshMat)
				m_MeshMat->Init();
		}

		void InitUniforms();

		void Draw(const Engine::Framework::Transform& transform) const;

		std::shared_ptr<Material>& GetMaterial() { return m_MeshMat; }

		void SetMesh(const std::shared_ptr<Engine::Rendering::MeshGPU>& mesh) { m_Mesh = mesh; }
		std::shared_ptr<Engine::Rendering::MeshGPU>& GetMesh() { return m_Mesh; }

		static std::shared_ptr<MeshRenderer> Create()
		{
			auto meshRenderer = std::make_shared<MeshRenderer>();
			CRTN_CHECK_PTR(meshRenderer);

			return meshRenderer;
		}
	private:
		std::shared_ptr<Engine::Rendering::MeshGPU> m_Mesh;
		std::shared_ptr<Material> m_MeshMat;

		MeshUniformLocations m_MeshUniforms;
	private:
		bool m_UniformsInitialized = true;
	};
}