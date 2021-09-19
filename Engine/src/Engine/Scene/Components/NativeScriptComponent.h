#pragma once

#include "Engine/Scene/ScriptableEntity.h"

namespace Engine
{
	struct NativeScriptComponent
	{
		ScriptableEntity* (*Create) ();
		void (*Destroy) (NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			Create = []() { return static_cast<ScriptableEntity*>(new T()); };
			Destroy = [](NativeScriptComponent* nsc) { delete nsc->m_Instance; nsc->m_Instance = nullptr; };
		}

	private:
		ScriptableEntity* m_Instance = nullptr;

		friend class Scene;
	};
}
