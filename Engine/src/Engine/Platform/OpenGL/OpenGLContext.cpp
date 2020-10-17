#include "OpenGLContext.h"

#include "Engine/Debug/Log.h"

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

		LOG_CORE_INFO("OpenGL Info:");
		LOG_CORE_INFO("\tVendor: {0}", glGetString(GL_VENDOR));
		LOG_CORE_INFO("\tRenderer: {0}", glGetString(GL_RENDERER));
		LOG_CORE_INFO("\tVersion: {0}", glGetString(GL_VERSION));
	}
	
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}
}
