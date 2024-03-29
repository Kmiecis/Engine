#pragma once

#include "Engine/Core/PlatformDetection.h"
#ifdef NG_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
#endif

// For use by applications
#include "Engine/Core/Application.h"
#include "Engine/Core/Base.h"
#include "Engine/Core/Layer.h"
#include "Engine/Debug/Log.h"

#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/MouseCodes.h"
#include "Engine/Renderer/OrthographicCameraController.h"

#include "Engine/ImGui/ImGuiLayer.h"

#include "Engine/Scene/Components.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Scene/ScriptableEntity.h"

// --- RENDERER -----------------------
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/RenderCommand.h"

#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/Framebuffer.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/SubTexture2D.h"
#include "Engine/Renderer/VertexArray.h"

#include "Engine/Renderer/OrthographicCamera.h"
// ------------------------------------
