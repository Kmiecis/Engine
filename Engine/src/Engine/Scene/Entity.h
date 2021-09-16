#pragma once

#include "Engine/Scene/Scene.h"

#include <entt.hpp>

namespace Engine
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			NG_CORE_ASSERT(!HasComponent<T>(), "Entity already has component");
			return m_Scene->GetRegistry().emplace<T>(m_Handle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			NG_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");
			return m_Scene->GetRegistry().get<T>(m_Handle);
		}

		template<typename T>
		bool HasComponent()
		{
			m_Scene->GetRegistry().has<T>(m_Handle);
		}

		template<typename T>
		void RemoveComponent()
		{
			NG_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");
			m_Scene->GetRegistry().remove<T>(m_Handle);
		}

		operator bool() const
		{
			return m_Handle != entt::null;
		}

	private:
		entt::entity m_Handle = entt::null;
		Scene* m_Scene = nullptr;
	};
}
