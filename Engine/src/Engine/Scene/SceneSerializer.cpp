#include "SceneSerializer.h"

#include "Engine/Debug/Log.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Components.h"

#include <yaml-cpp/yaml.h>
#include <string>
#include <fstream>
#include <sstream>

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& v)
		{
			Node node;
			node.push_back(v.x);
			node.push_back(v.y);
			node.push_back(v.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& v)
		{
			if (
				!node.IsSequence() ||
				node.size() != 3
			)
			{
				return false;
			}

			v.x = node[0].as<float>();
			v.y = node[1].as<float>();
			v.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& v)
		{
			Node node;
			node.push_back(v.x);
			node.push_back(v.y);
			node.push_back(v.z);
			node.push_back(v.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& v)
		{
			if (
				!node.IsSequence() ||
				node.size() != 4
				)
			{
				return false;
			}

			v.x = node[0].as<float>();
			v.y = node[1].as<float>();
			v.z = node[2].as<float>();
			v.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Engine
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) :
		m_Scene(scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;

		out << YAML::Key << "Entity" << YAML::Value << "999"; // TODO: Entity Id will go here

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent" << YAML::BeginMap;

			auto& component = entity.GetComponent<TagComponent>();
			out << YAML::Key << "Tag" << YAML::Value << component.Tag;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent" << YAML::BeginMap;

			auto& component = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << component.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << component.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << component.Scale;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent" << YAML::BeginMap;

			auto& component = entity.GetComponent<CameraComponent>();
			auto& camera = component.Camera;
			out << YAML::Key << "Camera" << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNear();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFar();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNear();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFar();
			out << YAML::EndMap;

			out << YAML::Key << "Primary" << YAML::Value << component.Primary;
			out << YAML::Key << "Fixed" << YAML::Value << component.Fixed;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent" << YAML::BeginMap;

			auto& component = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << component.Color;

			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const char* filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->GetRegistry().each([&](auto entityId)
		{
			Entity entity = Entity(entityId, m_Scene.get());
			if (entity)
			{
				SerializeEntity(out, entity);
			}
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout = std::ofstream(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const char* filepath)
	{
		NG_CORE_ASSERT(false);
	}

	bool SceneSerializer::Deserialize(const char* filepath)
	{
		std::ifstream stream = std::ifstream(filepath);
		std::stringstream sstream;
		sstream << stream.rdbuf();

		YAML::Node data = YAML::Load(sstream.str());
		if (!data["Scene"])
		{
			return false;
		}

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>(); // TODO

				std::string name = "Entity";

				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
				{
					name = tagComponent["Tag"].as<std::string>();
				}

				Entity deserializedEntity = m_Scene->CreateEntity(name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& component = deserializedEntity.GetComponent<TransformComponent>(); // We assume that every entity has it by default
					component.Translation = transformComponent["Translation"].as<glm::vec3>();
					component.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					component.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& component = deserializedEntity.AddComponent<CameraComponent>();

					auto& cameraProperties = cameraComponent["Camera"];
					component.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProperties["ProjectionType"].as<int>());

					component.Camera.SetPerspectiveVerticalFOV(cameraProperties["PerspectiveFOV"].as<float>());
					component.Camera.SetPerspectiveNear(cameraProperties["PerspectiveNear"].as<float>());
					component.Camera.SetPerspectiveFar(cameraProperties["PerspectiveFar"].as<float>());

					component.Camera.SetOrthographicSize(cameraProperties["OrthographicSize"].as<float>());
					component.Camera.SetOrthographicNear(cameraProperties["OrthographicNear"].as<float>());
					component.Camera.SetOrthographicFar(cameraProperties["OrthographicFar"].as<float>());

					component.Primary = cameraComponent["Primary"].as<bool>();
					component.Fixed = cameraComponent["Fixed"].as<bool>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& component = deserializedEntity.AddComponent<SpriteRendererComponent>();
					component.Color = spriteRendererComponent["Color"].as<glm::vec4>();
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const char* filepath)
	{
		NG_CORE_ASSERT(false);
		return false;
	}
}
