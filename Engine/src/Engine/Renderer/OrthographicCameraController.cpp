#include "OrthographicCameraController.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"

namespace Engine
{
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
        : m_AspectRatio(aspectRatio),
        m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
        m_Rotation(rotation)
    {
    }

    void OrthographicCameraController::OnUpdate(float dt)
    {
        if (Input::IsKeyPressed(KeyCode::A))
        {
            m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
            m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
        }
        else if (Input::IsKeyPressed(KeyCode::D))
        {
            m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
            m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
        }

        if (Input::IsKeyPressed(KeyCode::W))
        {
            m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
            m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
        }
        else if (Input::IsKeyPressed(KeyCode::S))
        {
            m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
            m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
        }

        if (m_Rotation)
        {
            if (Input::IsKeyPressed(KeyCode::Q))
                m_CameraRotation += m_CameraRotationSpeed * dt;
            if (Input::IsKeyPressed(KeyCode::E))
                m_CameraRotation -= m_CameraRotationSpeed * dt;

            if (m_CameraRotation > 180.0f)
                m_CameraRotation -= 360.0f;
            else if (m_CameraRotation <= -180.0f)
                m_CameraRotation += 360.0f;

            m_Camera.SetRotation(m_CameraRotation);
        }

        m_Camera.SetPosition(m_CameraPosition);

        m_CameraTranslationSpeed = m_ZoomLevel;
    }

    void OrthographicCameraController::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseScrolledEvent>(NG_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(NG_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    void OrthographicCameraController::Resize(float width, float height)
    {
        m_AspectRatio = width / height;
        CalculateView();
    }

    void OrthographicCameraController::SetZoomLevel(float level)
    {
        m_ZoomLevel = level;
        CalculateView();
    }

    void OrthographicCameraController::CalculateView()
    {
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
    {
        SetZoomLevel(std::max(m_ZoomLevel - event.GetYOffset() * m_CameraZoomSpeed, m_CameraZoomSpeed));
        return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
    {
        Resize((float)event.GetWidth(), (float)event.GetHeight());
        return false;
    }
}