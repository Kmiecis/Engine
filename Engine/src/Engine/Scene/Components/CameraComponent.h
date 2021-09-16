#pragma once

#include "Engine/Renderer/Camera.h"

namespace Engine
{
	struct CameraComponent
	{
		Camera Camera;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection) :
			Camera(projection)
		{
		}
	};
}
