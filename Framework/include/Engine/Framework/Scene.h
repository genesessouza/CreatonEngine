#pragma once

#include "Engine/Framework/Camera.h"
#include "Engine/Framework/GameObject.h"

#include <glad/glad.h>

namespace Engine::Framework
{
    class Scene
    {
    public:
        Scene()
        {
            glClearColor(0.3f, 0.7f, 0.9f, 1.0f);

            CRTN_LOG_INFO("[SCENE]: Background color set to: <Sky Blue>\n");
        }
        virtual ~Scene() = default;

        inline void OnViewportResize(uint32_t width, uint32_t height) 
        { 
            m_SceneCamera.SetViewportSize(width, height); 
        }

        void Render()
        {
            //CRTN_LOG_INFO("SceneRenderer::Render() called");

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            for (auto& obj : m_SceneObjects)
                obj->Draw(m_SceneCamera);
        }

    public:
        Camera m_SceneCamera;
        std::vector<std::unique_ptr<GameObject>> m_SceneObjects;
    };
}