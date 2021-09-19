#pragma once

#include "Engine/Scene/SceneCamera.h"

namespace Engine
{
	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool Fixed = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
}
