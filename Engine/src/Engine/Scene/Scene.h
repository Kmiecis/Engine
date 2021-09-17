#pragma once

#include "Engine/Core/Timestep.h"

#include <entt.hpp>
#include <string>

namespace Engine
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(Timestep timestep);
		void OnResize(uint32_t width, uint32_t height);

		entt::registry& GetRegistry();
		Entity CreateEntity(const std::string& name = "Entity");

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;
	};
}
