#pragma once

#include "Engine/Scene/Entity.h"

namespace Engine
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {};

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	protected:
		virtual void OnCreate() {};
		virtual void OnUpdate(float dt) {};
		virtual void OnDestroy() {};

	private:
		Entity m_Entity;

		friend class Scene;
	};
}
