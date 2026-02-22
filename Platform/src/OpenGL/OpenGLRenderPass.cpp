#include "Engine/Platform/OpenGL/OpenGLRenderPass.h"

#include <Engine/Rendering/RenderCommand.h>
#include <Engine/Rendering/MeshGPU.h>

#include <Engine/Core/Log/Logger.h>

#include <glad/glad.h>

namespace Engine::Platform::OpenGL
{
	// RENDER PASS
	void OpenGLRenderPass::Init(uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;
	}

	void OpenGLRenderPass::Begin()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glViewport(0, 0, m_Width, m_Height);
	}

	void OpenGLRenderPass::End()
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// SHADOW PASS
	void OpenGLShadowPass::Init(uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;

		m_DepthShader = Engine::Rendering::Shader::CreateDepthShader();
	}

	void OpenGLShadowPass::CalculateDirectionalLightSpaceMatrix(glm::vec3 lightDir, glm::vec3 cameraPos, float cameraNear, float cameraFar)
	{
		glm::vec3 dir = glm::normalize(lightDir);

		float frustumSize = 20.0f;

		// Lower/higher to ensure objects outside of camera sight are noticed
		float near_plane = cameraNear;
		float far_plane = cameraFar;

		m_LightProjection = glm::ortho(-frustumSize, frustumSize, -frustumSize, frustumSize, near_plane, far_plane);

		float shadowRange = cameraFar / 2;
		// Set light origin to be on camera pos and invert camera so it "sees" the scene
		glm::vec3 lightPos = cameraPos - (dir * shadowRange);

		glm::vec3 up = (std::abs(dir.y) > 0.99f) ? glm::vec3(1, 0, 0) : glm::vec3(0, 1, 0);

		// Light looks at where player's center is
		m_LightView = glm::lookAt(lightPos, cameraPos, up);

		m_LightSpaceMatrix = m_LightProjection * m_LightView;
	}

	void OpenGLShadowPass::Begin()
	{
		glViewport(0, 0, m_Width, m_Height);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		 
		glCullFace(GL_FRONT);
	}

	void OpenGLShadowPass::Render(const Engine::Rendering::RenderCommand& cmd)
	{
		m_DepthShader->Bind();

		m_DepthShader->DefineUniformMat4(m_DepthShader->GetUniformLocation(m_DepthShader->GetDefaultUniformNames(Engine::Rendering::UniformType::Model)), cmd.Transform);
		m_DepthShader->DefineUniformMat4(m_DepthShader->GetUniformLocation("u_LightSpaceMatrix"), m_LightSpaceMatrix);

		cmd.MeshPtr->Bind();
		glDrawElements(GL_TRIANGLES, cmd.MeshPtr->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLShadowPass::End()
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // Clearing
	}

	// POST PROCESS PASS
	void OpenGLPostProcessPass::Init(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0) return;

		m_Width = width;
		m_Height = height;

		if (m_VAO == 0)
		{
			float quadVertices[] =
			{
				// positions   // texCoords
				-1.0f,  1.0f,  0.0f, 1.0f,
				-1.0f, -1.0f,  0.0f, 0.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,

				-1.0f,  1.0f,  0.0f, 1.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,
				 1.0f,  1.0f,  1.0f, 1.0f
			};

			glGenVertexArrays(1, &m_VAO);
			glGenBuffers(1, &m_VBO);

			glBindVertexArray(m_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		}

		if (m_FBO)
		{
			glDeleteFramebuffers(1, &m_FBO);
			glDeleteTextures(1, &m_ColorTexture);
			glDeleteRenderbuffers(1, &m_RBO);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // Clearing

		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		glGenTextures(1, &m_ColorTexture);
		glBindTexture(GL_TEXTURE_2D, m_ColorTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture, 0);

		glGenRenderbuffers(1, &m_RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

		GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, buffers);

		CRTN_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Post Process FBO incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // Clearing

		m_Shader = Engine::Rendering::Shader::CreateTonemappingShader();
	}

	void OpenGLPostProcessPass::Begin()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glViewport(0, 0, m_Width, m_Height);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDisable(GL_DEPTH_TEST);
	}

	void OpenGLPostProcessPass::Render(uint32_t texture)
	{
		m_Shader->Bind();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		m_Shader->DefineUniformInt(m_Shader->GetUniformLocation("u_HDRBuffer"), 0);
		m_Shader->DefineUniformFloat(m_Shader->GetUniformLocation("u_Exposure"), m_Exposure);

		glBindVertexArray(m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void OpenGLPostProcessPass::End()
	{
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}