#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	struct TransformComponent
	{
		glm::mat4 Transform = glm::mat4(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		TransformComponent(const glm::mat4& transform)
			: Transform(transform)
		{
		}
	};
}
