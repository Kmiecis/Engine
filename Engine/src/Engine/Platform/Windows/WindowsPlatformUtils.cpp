#include "Engine/Utils/PlatformUtils.h"

#include "Engine/Core/Application.h"

#include <windows.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Engine
{
	std::string FileDialog::OpenFile(const char* filter)
	{
		OPENFILENAMEA openFileName;
		CHAR fileName[256] = { 0 };

		ZeroMemory(&openFileName, sizeof(OPENFILENAME));
		openFileName.lStructSize = sizeof(OPENFILENAME);
		openFileName.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		openFileName.lpstrFile = fileName;
		openFileName.nMaxFile = sizeof(fileName);
		openFileName.lpstrFilter = filter;
		openFileName.nFilterIndex = 1;
		openFileName.Flags = (
			OFN_PATHMUSTEXIST |
			OFN_FILEMUSTEXIST |
			OFN_NOCHANGEDIR
		);
		if (GetOpenFileNameA(&openFileName))
		{
			return openFileName.lpstrFile;
		}
		return std::string();
	}

	std::string FileDialog::SaveFile(const char* filter)
	{
		OPENFILENAMEA openFileName;
		CHAR fileName[256] = { 0 };

		ZeroMemory(&openFileName, sizeof(OPENFILENAME));
		openFileName.lStructSize = sizeof(OPENFILENAME);
		openFileName.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		openFileName.lpstrFile = fileName;
		openFileName.nMaxFile = sizeof(fileName);
		openFileName.lpstrFilter = filter;
		openFileName.nFilterIndex = 1;
		openFileName.Flags = (
			OFN_PATHMUSTEXIST |
			OFN_FILEMUSTEXIST |
			OFN_NOCHANGEDIR
		);
		if (GetSaveFileNameA(&openFileName))
		{
			return openFileName.lpstrFile;
		}
		return std::string();
	}
}
