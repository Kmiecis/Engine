#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	struct SpriteRendererComponent
	{
		glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;

		SpriteRendererComponent(const glm::vec4& color)
			: Color(color)
		{
		}
	};
}
