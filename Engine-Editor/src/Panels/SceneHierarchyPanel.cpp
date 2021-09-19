#include "SceneHierarchyPanel.h"

#include "Engine/Scene/Components.h"
#include "Engine/Scene/Entity.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
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

	void DrawVec3Control(const char* label, glm::vec3& values, float defaultValue)
	{
		const float COLUMN_WIDTH = 100.0f;
		const float DRAG_SPEED = 0.1f;
		const float DRAG_MIN = 0.0f;
		const float DRAG_MAX = 0.0f;
		const char* DRAG_FORMAT = "%.2f";
		const ImVec4 BUTTON_COLOR_RED = ImVec4(0.8f, 0.1f, 0.15f, 1.0f);
		const ImVec4 BUTTON_COLOR_RED_HOVERED = ImVec4(0.9f, 0.2f, 0.2f, 1.0f);
		const ImVec4 BUTTON_COLOR_GREEN = ImVec4(0.2f, 0.7f, 0.2f, 1.0f);
		const ImVec4 BUTTON_COLOR_GREEN_HOVERED = ImVec4(0.3f, 0.8f, 0.3f, 1.0f);
		const ImVec4 BUTTON_COLOR_BLUE = ImVec4(0.1f, 0.25f, 0.8f, 1.0f);
		const ImVec4 BUTTON_COLOR_BLUE_HOVERED = ImVec4(0.2f, 0.35f, 0.9f, 1.0f);

		ImGui::PushID(label);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, COLUMN_WIDTH);
		ImGui::Text(label);
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
		
		const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = ImVec2(lineHeight + 3.0f, lineHeight);
		
		ImGui::PushStyleColor(ImGuiCol_Button, BUTTON_COLOR_RED);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BUTTON_COLOR_RED_HOVERED);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, BUTTON_COLOR_RED);
		if (ImGui::Button("X", buttonSize))
		{
			values.x = defaultValue;
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, DRAG_SPEED, DRAG_MIN, DRAG_MAX, DRAG_FORMAT);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, BUTTON_COLOR_GREEN);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BUTTON_COLOR_GREEN_HOVERED);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, BUTTON_COLOR_GREEN);
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = defaultValue;
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, DRAG_SPEED, DRAG_MIN, DRAG_MAX, DRAG_FORMAT);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, BUTTON_COLOR_BLUE);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BUTTON_COLOR_BLUE_HOVERED);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, BUTTON_COLOR_BLUE);
		if (ImGui::Button("Z", buttonSize))
		{
			values.z = defaultValue;
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, DRAG_SPEED, DRAG_MIN, DRAG_MAX, DRAG_FORMAT);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
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
				auto& transformComponent = entity.GetComponent<TransformComponent>();

				DrawVec3Control("Translation", transformComponent.Translation, 0.0f);
				glm::vec3 rotation = glm::degrees(transformComponent.Rotation);
				DrawVec3Control("Rotation", rotation, 0.0f);
				transformComponent.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", transformComponent.Scale, 1.0f);

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
