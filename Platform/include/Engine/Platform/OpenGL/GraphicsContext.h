#pragma once

struct GLFWwindow;

namespace Engine::Platform
{
	class GraphicsContext
	{
	public:
		GraphicsContext(GLFWwindow* windowHandle);

		virtual void Init();
		virtual void SwapBuffers();
	private:
		GLFWwindow* m_WindowHandle;
	};
}