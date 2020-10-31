#pragma once

#include <Engine.h>

#include "ParticleSystem.h"

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
	Engine::Ref<Engine::Framebuffer> m_Framebuffer;

	glm::vec4 m_SquareColor = glm::vec4(0.2f, 0.3f, 0.8f, 1.0f);
};