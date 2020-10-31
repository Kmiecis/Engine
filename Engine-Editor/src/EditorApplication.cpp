#include <Engine/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Engine
{
	class EditorApplication : public Application
	{
	public:
		EditorApplication() : Application("Engine Editor")
		{
			PushLayer(new EditorLayer());
		}

		~EditorApplication()
		{
		}
	};

	Application* CreateApplication()
	{
		return new EditorApplication();
	}
}