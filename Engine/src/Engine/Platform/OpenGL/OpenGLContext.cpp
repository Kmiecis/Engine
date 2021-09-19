#include "OpenGLContext.h"

#include "Engine/Debug/Log.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

namespace Engine
{
    OpenGLContext::OpenGLContext(GLFWwindow* window)
        : m_Window(window)
    {
        NG_CORE_ASSERT(window, "Window is null!")
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_Window);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        NG_CORE_ASSERT(status, "Failed to initialize Glad!");

        NG_CORE_INFO("OpenGL Info:");
        NG_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        NG_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        NG_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

        NG_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Engine requires at least OpenGL version 4.5!");
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_Window);
    }
}