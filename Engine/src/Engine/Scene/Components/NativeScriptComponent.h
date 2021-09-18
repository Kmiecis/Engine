#pragma once

#include "Engine/Scene/ScriptableEntity.h"

#include <functional>

namespace Engine
{
	struct NativeScriptComponent
	{
		std::function<void()> Create;
		std::function<void()> Destroy;

		std::function<void()> OnCreate;
		std::function<void()> OnDestroy;
		std::function<void(float)> OnUpdate;

		template<typename T>
		void Bind()
		{
			Create = [&]() { m_Instance = new T(); };
			Destroy = [&]() { delete (T*)m_Instance; m_Instance = nullptr; };

			OnCreate = [&]() { ((T*)m_Instance)->OnCreate(); };
			OnDestroy = [&]() { ((T*)m_Instance)->OnDestroy(); };
			OnUpdate = [&](float dt) { ((T*)m_Instance)->OnUpdate(dt); };
		}

		bool IsCreated() const
		{
			return m_Instance != nullptr;
		}

	private:
		ScriptableEntity* m_Instance = nullptr;

		friend class Scene;
	};
}
