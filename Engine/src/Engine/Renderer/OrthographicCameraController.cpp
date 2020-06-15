#include "OrthographicCameraController.h"

#define NOMINMAX

#include "Engine/Core/Core.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/Log.h"

#include <algorithm>

namespace Engine
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_Rotation(rotation)
	{
	}
	
	void OrthographicCameraController::OnUpdate(Timestep timestep)
	{
		if (Input::IsKeyPressed(ENGINE_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * timestep;

		if (Input::IsKeyPressed(ENGINE_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * timestep;

		if (Input::IsKeyPressed(ENGINE_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * timestep;

		if (Input::IsKeyPressed(ENGINE_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * timestep;

		m_Camera.SetPosition(m_CameraPosition);

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(ENGINE_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * timestep;

			if (Input::IsKeyPressed(ENGINE_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * timestep;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_CameraTranslationSpeed = m_ZoomLevel;
	}
	
	void OrthographicCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}
	
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		m_ZoomLevel -= event.GetYOffset() * m_CameraZoomSpeed;
		m_ZoomLevel = std::max(m_ZoomLevel, m_CameraZoomSpeed);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
	
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}
