#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Engine
{
    EditorLayer::EditorLayer() : Layer("Editor"),
        m_CameraController(1280.0f / 720.0f)
    {
    }

    void EditorLayer::OnAttach()
    {
        m_CheckerBoardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
        m_WhiteTexture = Texture2D::Create("assets/textures/1x1.png");
        m_WhiteSubTexture = SubTexture2D::Create(m_WhiteTexture);

        FramebufferProperties framebufferProperties;
        framebufferProperties.Width = 1280;
        framebufferProperties.Height = 720;
        m_Framebuffer = Framebuffer::Create(framebufferProperties);

        m_ActiveScene = CreateRef<Scene>();
#if 0
        auto greenSquare = m_ActiveScene->CreateEntity("Green Square");
        greenSquare.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

        auto redSquare = m_ActiveScene->CreateEntity("Red Square");
        redSquare.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        auto camera = m_ActiveScene->CreateEntity("Camera");
        camera.AddComponent<CameraComponent>();

        class CameraController : public ScriptableEntity
        {
        public:
            void OnCreate()
            {
            }

            void OnDestroy()
            {
            }

            void OnUpdate(float dt)
            {
                auto& translation = GetComponent<TransformComponent>().Translation;
                const float SPEED = 5.0f;

                if (Input::IsKeyPressed(KeyCode::A))
                    translation.x -= SPEED * dt;
                if (Input::IsKeyPressed(KeyCode::D))
                    translation.x += SPEED * dt;
                if (Input::IsKeyPressed(KeyCode::W))
                    translation.y -= SPEED * dt;
                if (Input::IsKeyPressed(KeyCode::S))
                    translation.y += SPEED * dt;
            }
        };

        camera.AddComponent<NativeScriptComponent>()
            .Bind<CameraController>();
#endif

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnUpdate(float dt)
    {
        FramebufferProperties framebufferProperties = m_Framebuffer->GetProperties();
        if (
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
            (framebufferProperties.Width != m_ViewportSize.x || framebufferProperties.Height != m_ViewportSize.y)
        )
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_CameraController.Resize(m_ViewportSize.x, m_ViewportSize.y);
            m_ActiveScene->OnResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        // Update
        if (m_IsViewportFocused)
        {
            m_CameraController.OnUpdate(dt);
        }

        Renderer2D::ResetStats();

        // Render
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        RenderCommand::Clear();

        // Update scene
        m_ActiveScene->OnUpdate(dt);
        m_Framebuffer->Unbind();
    }

    void EditorLayer::OnImGuiRender()
    {
        static bool show_dockspace = true;
        if (show_dockspace)
        {
            static bool dockspace_open = false;
            static bool opt_fullscreen_persistant = true;
            bool opt_fullscreen = opt_fullscreen_persistant;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
            // because it would be confusing to have two docking targets within each others.
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            if (opt_fullscreen)
            {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->GetWorkPos());
                ImGui::SetNextWindowSize(viewport->GetWorkSize());
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }

            // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
            // and handle the pass-thru hole, so we ask Begin() to not render a background.
            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
            // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
            // all active windows docked into it will lose their parent and become undocked.
            // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
            // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags);
            ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // DockSpace
            const float WINDOW_MIN_WIDTH = 370.0f;

            ImGuiIO& io = ImGui::GetIO();
            ImGuiStyle& style = ImGui::GetStyle();
            float windowMinSizeX = style.WindowMinSize.x;
            style.WindowMinSize.x = WINDOW_MIN_WIDTH;
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }
            style.WindowMinSize.x = windowMinSizeX;

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Serialize"))
                    {
                        SceneSerializer serializer = SceneSerializer(m_ActiveScene);
                        serializer.Serialize("assets/scenes/Example.ng");
                    }
                    if (ImGui::MenuItem("Deserialize"))
                    {
                        SceneSerializer serializer = SceneSerializer(m_ActiveScene);
                        serializer.Deserialize("assets/scenes/Example.ng");
                    }
                    if (ImGui::MenuItem("Exit"))
                    {
                        Application::Get().Close();
                    }
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            m_SceneHierarchyPanel.OnImGuiRender();

            if (ImGui::Begin("Stats"))
            {
                auto stats = Renderer2D::GetStats();
                ImGui::Text("Renderer2D Stats:");
                ImGui::Text("Draw calls: %d", stats.DrawCalls);
                ImGui::Text("Quads: %d", stats.QuadCount);
                ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
                ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

                ImGui::End();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            if (ImGui::Begin("Viewport"))
            {
                m_IsViewportFocused = ImGui::IsWindowFocused();
                m_IsViewportHovered = ImGui::IsWindowHovered();
                Application::Get().GetImGuiLayer()->SetIsBlockingEvents(!m_IsViewportFocused || !m_IsViewportHovered);

                ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
                m_ViewportSize = glm::vec2(viewportPanelSize.x, viewportPanelSize.y);

                uint64_t textureId = m_Framebuffer->GetColorAttachmentRendererID();
                ImGui::Image(reinterpret_cast<void*>(textureId), ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

                ImGui::End();
            }
            ImGui::PopStyleVar();

            ImGui::End();
        }
    }

    void EditorLayer::OnEvent(Event& event)
    {
        m_CameraController.OnEvent(event);
    }
}
