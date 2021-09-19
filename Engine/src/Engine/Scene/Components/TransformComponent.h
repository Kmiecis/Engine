#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	struct TransformComponent
	{
		glm::vec3 Translation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		glm::mat4 GetTransform() const
		{
			const glm::mat4 IDENTITY = glm::mat4(1.0f);

			glm::mat4 translation = glm::translate(IDENTITY, Translation);
			glm::mat4 rotation = (
				glm::rotate(IDENTITY, Rotation.x, glm::vec3(1, 0, 0)) *
				glm::rotate(IDENTITY, Rotation.y, glm::vec3(0, 1, 0)) *
				glm::rotate(IDENTITY, Rotation.z, glm::vec3(0, 0, 1))
			);
			glm::mat4 scale = glm::scale(IDENTITY, Scale);

			return (
				translation *
				rotation *
				scale
			);
		}
	};
}
