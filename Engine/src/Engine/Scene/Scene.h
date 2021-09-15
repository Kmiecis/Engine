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

		entt::registry& GetRegistry();
		Entity CreateEntity(const std::string& name = "Entity");

	private:
		entt::registry m_Registry;
	};
}
