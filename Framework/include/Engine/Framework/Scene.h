#pragma once

#include <Engine/Core/Log/Logger.h>

#include "Engine/Framework/Camera.h"
#include "Engine/Framework/GameObject.h"
#include "Engine/Framework/Light.h"

#include "Engine/Framework/Physics/PhysicsComponent.h"

#include <Engine/Rendering/Renderer.h>

#include <glad/glad.h>

namespace Engine::Framework
{
    class Scene 
    {
    public:
        Scene() 
        {
            if (!s_ActiveScene) s_ActiveScene = this;
        }

        virtual ~Scene() 
        {
            if (s_ActiveScene == this) s_ActiveScene = nullptr;
        }

        static std::shared_ptr<Scene> Create() 
        {
            auto scene = std::make_shared<Scene>();
            CRTN_CHECK_PTR(scene);
            return scene;
		}

        static Scene& Get() 
        {
            CRTN_ASSERT(s_ActiveScene, "No active scene!");
            return *s_ActiveScene;
        }

        static void SetActiveScene(Scene* scene) { s_ActiveScene = scene; }

        void Init()
        {
			m_SceneCamera = CreateEntity<Camera>("[3D Camera] Scene Camera");

			m_SceneCamera->GetTransform().SetPosition({ 0.0f, 2.0f, 10.0f });
			//m_SceneCamera->GetTransform().SetRotation({ 20.0f, 0.0f, 0.0f });

			auto directionalLight = CreateEntity<Lights::DirectionalLight>("[Directional Light] Sun");
			directionalLight->GetTransform().SetPosition({ 1.0f, 5.0f, -3.0f });
        }

        template<typename T, typename... Args>
        std::shared_ptr<T> CreateEntity(const std::string& name, Args&&... args) {
            std::shared_ptr<T> entity = std::make_shared<T>(std::forward<Args>(args)...);
            entity->SetName(name);
            m_Entities.push_back(entity);
            return entity;
        }

        void AddEntity(std::shared_ptr<Entity> entity) { m_Entities.push_back(entity); }

		std::shared_ptr<Camera> GetSceneCamera() const { return m_SceneCamera; }

        const std::vector<std::shared_ptr<Entity>>& GetEntities() const { return m_Entities; }
		const std::vector<std::shared_ptr<Lights::PointLight>> GetPointLights() const;
		const std::shared_ptr<Lights::DirectionalLight> GetDirectionalLight() const;

        void OnUpdateRuntime(float ts);
        void ResolveCollision(Physics::PhysicsComponent* physA, Physics::PhysicsComponent* physB, const glm::vec3& overlap, const glm::vec3& hitPoint);

        void OnRender(std::shared_ptr<Camera> camera);

		void OnViewportResize(uint32_t width, uint32_t height);
    protected:
        std::shared_ptr<Camera> m_SceneCamera;
    private:
        std::vector<std::shared_ptr<Entity>> m_Entities;
        static Scene* s_ActiveScene;
    };
}