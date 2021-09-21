#pragma once

#include "Panels/SceneHierarchyPanel.h"

#include <Engine.h>

namespace Engine
{
    class EditorLayer : public Layer
    {
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;

        void OnAttach() override;
        void OnDetach() override;

        void OnUpdate(float dt) override;
        void OnImGuiRender() override;
        void OnEvent(Event& event) override;

    private:
        bool OnKeyPressed(KeyPressedEvent& e);
        void NewScene();
        void OpenScene();
        void SaveScene();

    private:
        Ref<Scene> m_ActiveScene;

        Ref<Framebuffer> m_Framebuffer;

        bool m_IsViewportFocused = false;
        bool m_IsViewportHovered = false;
        glm::vec2 m_ViewportSize = glm::vec2(0.0f, 0.0f);

        SceneHierarchyPanel m_SceneHierarchyPanel;
    };
}
