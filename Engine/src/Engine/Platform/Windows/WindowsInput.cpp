#include "Engine/Core/Application.h"
#include "Engine/Core/Input.h"

#include <GLFW/glfw3.h>

namespace Engine
{
    bool Input::IsKeyPressed(KeyCode key)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, static_cast<int32_t>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(MouseCode button)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
        return state == GLFW_PRESS;
    }

    glm::vec2 Input::GetMousePosition()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return glm::vec2(xpos, ypos);
    }

    float Input::GetMouseX()
    {
        glm::vec2 mousePosition = GetMousePosition();
        return mousePosition.x;
    }

    float Input::GetMouseY()
    {
        glm::vec2 mousePosition = GetMousePosition();
        return mousePosition.y;
    }
}