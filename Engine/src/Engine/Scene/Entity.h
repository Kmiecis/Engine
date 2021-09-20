#pragma once

#include "Engine/Core/Base.h"
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
			T& component = m_Scene->GetRegistry().emplace<T>(m_Handle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
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
			return m_Scene->GetRegistry().any_of<T>(m_Handle);
		}

		template<typename T>
		void RemoveComponent()
		{
			NG_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");
			m_Scene->GetRegistry().remove<T>(m_Handle);
		}

		operator entt::entity() const
		{
			return m_Handle;
		}

		operator bool() const
		{
			return m_Handle != entt::null;
		}

		operator uint32_t() const
		{
			return (uint32_t)m_Handle;
		}

		bool operator==(const Entity& other) const
		{
			return (
				m_Handle == other.m_Handle &&
				m_Scene == other.m_Scene
			);
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		entt::entity m_Handle = entt::null;
		Scene* m_Scene = nullptr;
	};
}
