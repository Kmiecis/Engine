#pragma once

#include "Engine/Scene/SceneCamera.h"

namespace Engine
{
	struct CameraComponent
	{
		SceneCamera Camera;
		bool Fixed = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
}
