#include "SceneHierarchyPanel.h"

#include "Engine/Scene/Components.h"
#include "Engine/Scene/Entity.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Engine
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		if (ImGui::Begin("Scene Hierarchy"))
		{
			auto& registry = m_Context->GetRegistry();
			registry.each([&](auto entityID)
			{
				Entity entity = Entity(entityID, m_Context.get());
				DrawEntityNode(entity);
			});

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			{
				m_SelectionContext = Entity();
			}

			ImGui::End();
		}

		if (ImGui::Begin("Properties"))
		{
			if (m_SelectionContext)
			{
				DrawComponents(m_SelectionContext);
			}

			ImGui::End();
		}
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = (m_SelectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str()))
		{
			ImGui::TreePop();
		}
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, tag.c_str());

			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>().Transform;

				const float DRAG_STEP = 0.1f;
				if (ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), DRAG_STEP))
				{
					// No need to do anything at this point...
				}

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.Camera;

				ImGui::Checkbox("Primary", &cameraComponent.Primary);

				const int PROJECTION_TYPE_STRINGS_COUNT = 2;
				const char* PROJECTION_TYPE_STRINGS[PROJECTION_TYPE_STRINGS_COUNT] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = PROJECTION_TYPE_STRINGS[(int)camera.GetProjectionType()];

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < PROJECTION_TYPE_STRINGS_COUNT; ++i)
					{
						bool isSelected = currentProjectionTypeString == PROJECTION_TYPE_STRINGS[i];
						if (ImGui::Selectable(PROJECTION_TYPE_STRINGS[i], isSelected))
						{
							currentProjectionTypeString = PROJECTION_TYPE_STRINGS[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					const float DRAG_STEP = 0.1f;

					auto fov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &fov, DRAG_STEP))
					{
						camera.SetPerspectiveVerticalFOV(glm::radians(fov));
					}

					auto near = camera.GetPerspectiveNear();
					if (ImGui::DragFloat("Near", &near, DRAG_STEP))
					{
						camera.SetPerspectiveNear(near);
					}

					auto far = camera.GetPerspectiveFar();
					if (ImGui::DragFloat("Far", &far, DRAG_STEP))
					{
						camera.SetPerspectiveFar(far);
					}
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					const float DRAG_STEP = 0.1f;

					auto size = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &size, DRAG_STEP))
					{
						camera.SetOrthographicSize(size);
					}

					auto near = camera.GetOrthographicNear();
					if (ImGui::DragFloat("Near", &near, DRAG_STEP))
					{
						camera.SetOrthographicNear(near);
					}

					auto far = camera.GetOrthographicFar();
					if (ImGui::DragFloat("Far", &far, DRAG_STEP))
					{
						camera.SetOrthographicFar(far);
					}

					ImGui::Checkbox("Fixed", &cameraComponent.Fixed);
				}

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
			{
				auto& color = entity.GetComponent<SpriteRendererComponent>().Color;

				ImGui::ColorEdit4("Color", glm::value_ptr(color));

				ImGui::TreePop();
			}
		}
	}
}
