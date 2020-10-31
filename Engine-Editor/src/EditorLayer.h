#pragma once

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

		void OnUpdate(Timestep timestep) override;
		void OnImGuiRender() override;
		void OnEvent(Event& event) override;

	private:
		OrthographicCameraController m_CameraController;

		Ref<Texture2D> m_CheckerBoardTexture;
		Ref<Texture2D> m_WhiteTexture;
		Ref<SubTexture2D> m_WhiteSubTexture;
		Ref<Framebuffer> m_Framebuffer;

		bool m_IsViewportFocused = false;
		bool m_IsViewportHovered = false;
		glm::vec2 m_ViewportSize = glm::vec2(0.0f, 0.0f);
	};
}