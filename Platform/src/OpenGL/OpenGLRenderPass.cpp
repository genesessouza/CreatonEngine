#include "Engine/Platform/OpenGL/OpenGLRenderPass.h"

#include <Engine/Rendering/RenderCommand.h>
#include <Engine/Rendering/MeshGPU.h>

#include <Engine/Core/Log/Logger.h>

#include <glad/glad.h>

namespace Engine::Platform::OpenGL
{
	// RENDER PASS
	void OpenGLRenderPass::Init()
	{
	}

	void OpenGLRenderPass::Begin()
	{

	}

	void OpenGLRenderPass::End()
	{

	}

	// SHADOW PASS
	void OpenGLShadowPass::Init(uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;

		glCreateFramebuffers(1, &m_FBO);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthTexture);
		glBindTexture(GL_TEXTURE_2D, m_DepthTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		CRTN_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Shadow FBO incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_DepthShader = Engine::Rendering::Shader::CreateDepthShader();
	}

	void OpenGLShadowPass::Begin()
	{
		glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		glViewport(0, 0, m_Width, m_Height);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		glCullFace(GL_FRONT); // evita shadow acne
	}

	void OpenGLShadowPass::End()
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCullFace(GL_BACK);
	}

	void OpenGLShadowPass::Render(const Engine::Rendering::RenderCommand& cmd)
	{
		m_DepthShader->Bind();

		m_DepthShader->DefineUniformMat4(m_DepthShader->GetUniformLocation(m_DepthShader->GetDefaultUniformNames(Engine::Rendering::UniformType::Model)), cmd.Transform);
		m_DepthShader->DefineUniformMat4(m_DepthShader->GetUniformLocation("u_LightSpaceMatrix"), m_LightSpaceMatrix);

		cmd.MeshPtr->Bind();
		glDrawElements(GL_TRIANGLES, cmd.MeshPtr->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	}

	// POST PROCESS PASS
	void OpenGLPostProcessPass::Init(uint32_t width, uint32_t height)
	{
		float quadVertices[] = {
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

		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		glGenTextures(1, &m_ColorTexture);
		glBindTexture(GL_TEXTURE_2D, m_ColorTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture, 0);

		glGenRenderbuffers(1, &m_RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

		GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, buffers);

		CRTN_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Shadow FBO incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_Shader = Engine::Rendering::Shader::CreateTonemappingShader();
	}

	void OpenGLPostProcessPass::Begin()
	{
		glViewport(0, 0, m_Width, m_Height);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLPostProcessPass::End()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLPostProcessPass::Render(uint32_t texture)
	{
		m_Shader->Bind();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		m_Shader->DefineUniformInt(m_Shader->GetUniformLocation("u_HDRBuffer"), 0);

		glBindVertexArray(m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}