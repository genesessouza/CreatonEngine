#include "Engine/Platform/OpenGL/OpenGLRendererAPI.h"

#include <Engine/Rendering/Renderer.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Engine::Platform::OpenGL
{
	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void OpenGLRendererAPI::DrawHitpoint(const glm::vec3& center, float radius, const glm::vec4& color, const glm::mat4& viewMatrix, const glm::mat4& projMatrix)
	{
		/*
		glUseProgram(0);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(projMatrix));
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(viewMatrix));

		glColor4f(color.r, color.g, color.b, color.a);
		glLineWidth(2.0f);

		int segments = 32;
		float step = 2.0f * glm::pi<float>() / (float)segments;

		for (int ring = 0; ring < 3; ring++)
		{
			glBegin(GL_LINE_LOOP);
			for (int i = 0; i < segments; i++)
			{
				float angle = i * step;
				float x = radius * cos(angle);
				float y = radius * sin(angle);

				if (ring == 0)      glVertex3f(center.x + x, center.y + y, center.z); // Anel XY
				else if (ring == 1) glVertex3f(center.x + x, center.y, center.z + y); // Anel XZ
				else                glVertex3f(center.x, center.y + x, center.z + y); // Anel YZ
			}
			glEnd();
		}

		glLineWidth(1.0f);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		*/
	}

	void OpenGLRendererAPI::DrawLine(const glm::vec3& startingPoint, const glm::vec3& endPoint, const glm::vec4& color, const glm::mat4& viewMatrix, const glm::mat4& projMatrix)
	{
		/*
		glUseProgram(0);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(projMatrix));
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glm::value_ptr(viewMatrix));

		glBegin(GL_LINES);
		glLineWidth(2.0f);
		glColor4f(color.r, color.g, color.b, color.a);
		glVertex3f(startingPoint.x, startingPoint.y, startingPoint.z);
		glVertex3f(endPoint.x, endPoint.y, endPoint.z);
		glEnd();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		*/
	}
}