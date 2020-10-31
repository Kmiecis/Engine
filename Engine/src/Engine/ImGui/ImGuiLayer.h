#pragma once

#include "Engine/Core/Layer.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"

namespace Engine
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		void SetIsBlockingEvents(bool isBlockingEvents) { m_IsBlockingEvents = isBlockingEvents; }

	private:
		bool m_IsBlockingEvents = true;
		float m_Time = 0.0f;
	};
}