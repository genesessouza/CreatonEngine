#include "Engine/Framework/Entity.h"

#include <Engine/Rendering/MeshRenderer.h>

namespace Engine::Framework
{
	Entity::Entity(const char* name) : m_Name(name), m_IsEnabled(true) {}

	Entity::Entity(const char* name, const std::shared_ptr<Engine::Rendering::MeshGPU>& mesh)
		: Entity(name)
	{
		auto meshRenderer = AddComponent<Engine::Rendering::MeshRenderer>();
		meshRenderer->Init();

		if (!mesh || mesh->GetMeshCPU().GetMeshType() == Geometry::Mesh::MeshType::NONE)
			meshRenderer->SetMesh(MeshLibrary::InstantiateCube());
		else
			meshRenderer->SetMesh(mesh);

		GetTransform().SetPosition(glm::vec3(0.0f));
		GetTransform().SetRotation(glm::vec3(0.0f));
		GetTransform().SetScale(glm::vec3(1.0f));
	}

	void Entity::OnUpdate()
	{
		for (auto& comp : m_Components)
			comp->OnUpdate();
	}
}