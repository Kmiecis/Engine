#include "Scene.h"

#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Scene/Components.h"
#include "Engine/Scene/Entity.h"

namespace Engine
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	entt::registry& Scene::GetRegistry()
	{
		return m_Registry;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = Entity(m_Registry.create(), this);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(name);
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(float dt)
	{
		// Scripts
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
		{
			if (!nsc.m_Instance)
			{
				nsc.m_Instance = nsc.Create();
				nsc.m_Instance->m_Entity = Entity(entity, this);
				nsc.m_Instance->OnCreate();
			}

			nsc.m_Instance->OnUpdate(dt);
		});


		// Render
		CameraComponent* mainCameraComponent = nullptr;
		TransformComponent* mainTransformComponent = nullptr;

		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view)
		{
			auto& transformComponent = view.get<TransformComponent>(entity);
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (cameraComponent.Primary)
			{
				mainCameraComponent = &cameraComponent;
				mainTransformComponent = &transformComponent;
				break;
			}
		}

		if (mainCameraComponent)
		{
			Renderer2D::BeginScene(mainCameraComponent->Camera.GetProjection(), mainTransformComponent->GetTransform());

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.Fixed)
			{
				auto& camera = cameraComponent.Camera;
				camera.SetViewport(width, height);
			}
		}
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewport(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}
}
