#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	SceneCamera::SceneCamera()
	{
		Recalculate();
	}

	SceneCamera::ProjectionType SceneCamera::GetProjectionType() const
	{
		return m_ProjectionType;
	}

	void SceneCamera::SetProjectionType(SceneCamera::ProjectionType type)
	{
		m_ProjectionType = type;

		Recalculate();
	}

	float SceneCamera::GetOrthographicSize() const
	{
		return m_OrthographicSize;
	}

	void SceneCamera::SetOrthographicSize(float size)
	{
		m_OrthographicSize = size;

		Recalculate();
	}

	float SceneCamera::GetOrthographicNear() const
	{
		return m_OrthographicNear;
	}

	void SceneCamera::SetOrthographicNear(float near)
	{
		m_OrthographicNear = near;

		Recalculate();
	}

	float SceneCamera::GetOrthographicFar() const
	{
		return m_OrthographicFar;
	}

	void SceneCamera::SetOrthographicFar(float far)
	{
		m_OrthographicFar = far;

		Recalculate();
	}

	void SceneCamera::SetOrthographic(float size, float near, float far)
	{
		m_ProjectionType = SceneCamera::ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = near;
		m_OrthographicFar = far;

		Recalculate();
	}

	float SceneCamera::GetPerspectiveVerticalFOV() const
	{
		return m_PerspectiveFOV;
	}

	void SceneCamera::SetPerspectiveVerticalFOV(float fov)
	{
		m_PerspectiveFOV = fov;

		Recalculate();
	}

	float SceneCamera::GetPerspectiveNear() const
	{
		return m_PerspectiveNear;
	}

	void SceneCamera::SetPerspectiveNear(float near)
	{
		m_PerspectiveNear = near;

		Recalculate();
	}

	float SceneCamera::GetPerspectiveFar() const
	{
		return m_PerspectiveFar;
	}

	void SceneCamera::SetPerspectiveFar(float far)
	{
		m_PerspectiveFar = far;

		Recalculate();
	}

	void SceneCamera::SetPerspective(float fov, float near, float far)
	{
		m_ProjectionType = SceneCamera::ProjectionType::Perspective;
		m_PerspectiveFOV = fov;
		m_PerspectiveNear = near;
		m_PerspectiveFar = far;

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
		if (m_ProjectionType == ProjectionType::Perspective)
		{
			m_Projection = glm::perspective(
				m_PerspectiveFOV, m_AspectRatio,
				m_PerspectiveNear, m_PerspectiveFar
			);
		}
		else
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
}
