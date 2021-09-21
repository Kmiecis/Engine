#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Scene/Scene.h"

namespace Engine
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const char* filepath);
		void SerializeRuntime(const char* filepath);

		bool Deserialize(const char* filepath);
		bool DeserializeRuntime(const char* filepath);

	private:
		Ref<Scene> m_Scene;
	};
}
