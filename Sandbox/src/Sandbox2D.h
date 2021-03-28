#pragma once

#include <Engine.h>

class Sandbox2D : public Engine::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(Engine::Timestep timestep) override;
    void OnImGuiRender() override;
    void OnEvent(Engine::Event& event) override;

private:
    Engine::OrthographicCameraController m_CameraController;

    Engine::Ref<Engine::Texture2D> m_CheckerBoardTexture;
    Engine::Ref<Engine::Texture2D> m_WhiteTexture;
    Engine::Ref<Engine::SubTexture2D> m_WhiteSubTexture;
};