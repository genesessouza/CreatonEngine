#pragma once

#include "Engine/Framework/Transform.h"
#include "Engine/Framework/Component.h"
#include "Engine/Framework/Scene.h"
#include "Engine/Framework/MeshLibrary.h"
#include "Engine/Framework/Billboard.h"

#include "Engine/Framework/Physics/Collider.h"
#include "Engine/Framework/Physics/PhysicsComponent.h"

#include <memory>
#include <string>

namespace Engine::Rendering
{
	class MeshRenderer;
}

namespace Engine::Framework
{
	class Entity
	{
	public:
		Entity(const char* name);
		Entity(const char* name, const std::shared_ptr<Engine::Rendering::MeshGPU>& mesh);

		virtual ~Entity() = default;

		void OnUpdate();

		void SetEnabled(bool enabled) { m_IsEnabled = enabled; }
		bool IsEnabled() const { return m_IsEnabled; }

		const std::string& GetName() const { return m_Name; }
		void SetName(const std::string& name) { m_Name = name; }

		Transform& GetTransform() { return m_Transform; }
		const Transform& GetTransform() const { return m_Transform; }

		bool HasExtraComponents() const { return m_Components.size() > 1; /* Mesh Renderer does not count */ }

		template<typename T>
		T* GetComponent()
		{
			if constexpr (std::is_same_v<T, Transform>)
				return &m_Transform;

			for (auto& c : m_Components)
			{
				if (auto casted = dynamic_cast<T*>(c.get()))
					return casted;
			}

			return nullptr;
		}

		template<typename T>
		std::vector<T*> GetComponents()
		{
			std::vector<T*> result;

			for (auto& comp : m_Components)
			{
				if (auto casted = dynamic_cast<T*>(comp.get()))
					result.push_back(casted);
			}

			return result;
		}

		template<typename T, typename... Args>
		T* AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of_v<Component, T>,
				"T must derive from Component");

			static_assert(!std::is_abstract_v<T>,
				"Cannot instantiate abstract Component");

			auto comp = std::make_unique<T>(std::forward<Args>(args)...);
			T* raw = comp.get();
			raw->SetOwner(this);

			m_Components.push_back(std::move(comp));

			return raw;
		}

		static std::unique_ptr<Entity> CreateEmpty(const char* name)
		{
			return std::make_unique<Entity>(name);
		}

		static std::unique_ptr<Entity> CreateBillboard(const char* name, float size, const glm::vec4& color)
		{
			auto entity = std::make_unique<Entity>(name);

			auto billboard = entity->AddComponent<Billboard>();
			billboard->Init();
			billboard->SetSize(size);
			billboard->SetColor(color);

			return entity;
		}

		static std::unique_ptr<Entity> CreateWithRenderer(const char* name, const std::shared_ptr<Engine::Rendering::MeshGPU>& mesh)
		{
			return std::make_unique<Entity>(name, mesh);
		}

		static std::unique_ptr<Entity> CreateWithCollider(const char* name, const std::shared_ptr<Engine::Rendering::MeshGPU>& mesh, bool enabled = true)
		{
			auto entity = std::make_unique<Entity>(name, mesh);

			auto type = mesh->GetMeshCPU().GetMeshType();

			if (type == Geometry::Mesh::MeshType::CUBE)
			{
				auto collider = entity->AddComponent<Physics::CubeCollider>();
				collider->SetEnabled(enabled);
				collider->SetStatic(true);
			}
			else if (type == Geometry::Mesh::MeshType::SPHERE)
			{
				auto collider = entity->AddComponent<Physics::SphereCollider>();
				collider->SetEnabled(enabled);
				collider->SetStatic(true);
			}

			return entity;
		}

		static std::unique_ptr<Entity> CreateWithPhysics(const char* name, const std::shared_ptr<Engine::Rendering::MeshGPU>& mesh, bool enabled = true)
		{
			auto entity = CreateWithCollider(name, mesh, true);

			auto physics = entity->AddComponent<Physics::PhysicsComponent>();
			physics->SetMass(10.0f);
			physics->SetStatic(enabled);

			return entity;
		}

	private:
		std::string m_Name;
		bool m_IsEnabled;

		Transform m_Transform;

		std::vector<std::unique_ptr<Component>> m_Components;

		Scene* m_Scene = nullptr;
	};
}