#pragma once

#include "Engine/Renderer/Camera.h"

namespace Engine
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float near, float far);
		void SetViewport(float aspectRatio);
		void SetViewport(uint32_t width, uint32_t height);

	private:
		void Recalculate();

	private:
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar = 1.0f;
		float m_AspectRatio = 1.0f;
	};
}
