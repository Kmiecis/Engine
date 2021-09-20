#pragma once

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

		void OnUpdate(float dt);
		void OnResize(uint32_t width, uint32_t height);

		entt::registry& GetRegistry();
		Entity CreateEntity(const std::string& name = "Entity");
		void DestroyEntity(Entity entity);

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		friend class Entity;
	};
}
