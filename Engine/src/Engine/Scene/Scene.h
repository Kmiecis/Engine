#pragma once

#include "Engine/Core/Timestep.h"

#include <entt.hpp>

namespace Engine
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::registry& GetRegistry();
		entt::entity CreateEntity();
		void OnUpdate(Timestep timestep);

	private:
		entt::registry m_Registry;
	};
}
