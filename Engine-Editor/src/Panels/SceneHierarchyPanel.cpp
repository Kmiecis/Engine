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

			// Right-click on blank space
			if (ImGui::BeginPopupContextWindow(0, 1, false))
			{
				if (ImGui::MenuItem("Create new Entity"))
				{
					m_Context->CreateEntity();
				}

				ImGui::EndPopup();
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

		ImGuiTreeNodeFlags flags = 0;
		if (m_SelectionContext == entity)
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}
		flags |= ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str()))
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			if (ImGui::TreeNodeEx((void*)999, flags, tag.c_str()))
			{
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				if (m_SelectionContext == entity)
				{
					m_SelectionContext = Entity();
				}
				m_Context->DestroyEntity(entity);
			}

			ImGui::EndPopup();
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

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

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
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
		{
			values.x = defaultValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, DRAG_SPEED, DRAG_MIN, DRAG_MAX, DRAG_FORMAT);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, BUTTON_COLOR_GREEN);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BUTTON_COLOR_GREEN_HOVERED);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, BUTTON_COLOR_GREEN);
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = defaultValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, DRAG_SPEED, DRAG_MIN, DRAG_MAX, DRAG_FORMAT);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, BUTTON_COLOR_BLUE);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BUTTON_COLOR_BLUE_HOVERED);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, BUTTON_COLOR_BLUE);
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
		{
			values.z = defaultValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, DRAG_SPEED, DRAG_MIN, DRAG_MAX, DRAG_FORMAT);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void TryDrawComponent(const char* name, Entity entity, UIFunction function)
	{
		const ImGuiTreeNodeFlags TREE_NODE_FLAGS = (
			ImGuiTreeNodeFlags_DefaultOpen |
			ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_FramePadding |
			ImGuiTreeNodeFlags_AllowItemOverlap
		);

		if (entity.HasComponent<T>())
		{
			const ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool isUnfolded = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), TREE_NODE_FLAGS, name);
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2(lineHeight, lineHeight)))
			{
				ImGui::OpenPopup("Component Settings");
			}

			bool hasRemovedComponent = false;
			if (ImGui::BeginPopup("Component Settings"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					hasRemovedComponent = true;
				}

				ImGui::EndPopup();
			}

			if (isUnfolded)
			{
				auto& component = entity.GetComponent<T>();

				function(component);

				ImGui::TreePop();
			}

			if (hasRemovedComponent)
			{
				entity.RemoveComponent<T>();
			}
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

			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("Add Component");
		}

		if (ImGui::BeginPopup("Add Component"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				m_SelectionContext.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Sprite Renderer"))
			{
				m_SelectionContext.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();

		TryDrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			DrawVec3Control("Translation", component.Translation, 0.0f);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation, 0.0f);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale, 1.0f);
		});

		TryDrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			auto& camera = component.Camera;

			ImGui::Checkbox("Primary", &component.Primary);

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

				ImGui::Checkbox("Fixed", &component.Fixed);
			}
		});

		TryDrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
		});
	}
}
