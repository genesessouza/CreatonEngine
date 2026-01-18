#pragma once

#include "Engine/Framework/Camera.h"
#include "Engine/Framework/GameObject.h"

namespace Engine::Framework
{
    class Scene
    {
    public:
        virtual ~Scene() = default;

        inline void OnViewportResize(uint32_t width, uint32_t height) 
        { 
            m_SceneCamera.SetViewportSize(width, height); 
        }

        void Render()
        {
            //CRTN_LOG_INFO("SceneRenderer::Render() called");

            for (auto& obj : m_SceneObjects)
                obj->Draw(m_SceneCamera);
        }

    public:
        Camera m_SceneCamera;
        std::vector<std::unique_ptr<GameObject>> m_SceneObjects;
    };
}