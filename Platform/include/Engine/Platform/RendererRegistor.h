#pragma once

namespace Engine::Core
{
	class RendererFactory;
	
	namespace Log { class Logger; }
}

namespace Engine::Platform
{
	enum API { None, OpenGl, Vulkan };

	namespace OpenGL { class OpenGLRendererAPI; }
}

void RegisterRendererAPI(Engine::Platform::API targetApi);