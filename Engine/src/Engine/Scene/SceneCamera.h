#pragma once

#include "Engine/Renderer/Camera.h"

namespace Engine
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType
		{
			Perspective = 0,
			Orthographic = 1
		};

	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		ProjectionType GetProjectionType() const;
		void SetProjectionType(ProjectionType type);

		float GetOrthographicSize() const;
		void SetOrthographicSize(float size);

		float GetOrthographicNear() const;
		void SetOrthographicNear(float near);

		float GetOrthographicFar() const;
		void SetOrthographicFar(float far);

		void SetOrthographic(float size, float near, float far);

		float GetPerspectiveVerticalFOV() const;
		void SetPerspectiveVerticalFOV(float fov);

		float GetPerspectiveNear() const;
		void SetPerspectiveNear(float near);

		float GetPerspectiveFar() const;
		void SetPerspectiveFar(float far);

		void SetPerspective(float fov, float near, float far);

		void SetViewport(float aspectRatio);
		void SetViewport(uint32_t width, uint32_t height);

	private:
		void Recalculate();

	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f;
		float m_PerspectiveFar = 1000.0f;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar = 1.0f;

		float m_AspectRatio = 1.0f;
	};
}
