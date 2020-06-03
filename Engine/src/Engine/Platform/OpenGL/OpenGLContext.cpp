#include "OpenGLContext.h"

#include "Engine/Log.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Engine
{
	OpenGLContext::OpenGLContext(GLFWwindow* window)
		: m_Window(window)
	{
		LOG_CORE_ASSERT(window, "Window is null!");
	}
	
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		LOG_CORE_ASSERT(status, "Failed to initialize Glad!");
	}
	
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}
}
