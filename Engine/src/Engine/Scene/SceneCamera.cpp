#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	SceneCamera::SceneCamera()
	{
		Recalculate();
	}

	void SceneCamera::SetOrthographic(float size, float near, float far)
	{
		m_OrthographicSize = size;
		m_OrthographicNear = near;
		m_OrthographicFar = far;

		Recalculate();
	}

	void SceneCamera::SetViewport(float aspectRatio)
	{
		m_AspectRatio = aspectRatio;

		Recalculate();
	}

	void SceneCamera::SetViewport(uint32_t width, uint32_t height)
	{
		SetViewport((float)width / (float)height);
	}

	void SceneCamera::Recalculate()
	{
		float orthographicRight = m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthographicLeft = -orthographicRight;
		float orthographicTop = m_OrthographicSize * 0.5f;
		float orthographicBottom = -orthographicTop;
		m_Projection = glm::ortho(
			orthographicLeft, orthographicRight,
			orthographicTop, orthographicBottom,
			m_OrthographicNear, m_OrthographicFar
		);
	}
}
