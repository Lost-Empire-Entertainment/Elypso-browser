//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <iostream>
#include <filesystem>
#include <locale>
#include <codecvt>

//external
#include "glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw3native.h"

//browser
#include "content.hpp"
#include "core.hpp"
#include "render.hpp"

using Core::Browser;

using std::cout;
using std::filesystem::path;
using std::filesystem::exists;
using std::wstring;

namespace Graphics
{
	void Content::Initialize()
	{
		//
		// INITIALIZE SCITER
		//

		//load sciter dll
		HMODULE hSciter = ::LoadLibrary("sciter.dll");
		if (!hSciter)
		{
			cout << "Error: Failed to load sciter.dll!\n";
			return;
		}
		else cout << "Successfully loaded sciter.dll!\n";

		//fetch ScipterAPI function address
		auto pScipterAPI = (SciterAPI_ptr)::GetProcAddress(hSciter, "SciterAPI");
		if (!pScipterAPI)
		{
			cout << "Error: Failed to get SciterAPI function address!\n";
			::FreeLibrary(hSciter);
			return;
		}
		else cout << "Successfully found SciterAPI function address!\n";

		//call the function to retrieve the pointer
		ISciterAPI* sciterAPI = pScipterAPI();
		if (!sciterAPI)
		{
			cout << "Error: SciterAPI() returned nullptr!\n";
			::FreeLibrary(hSciter);
			return;
		}
		else cout << "Successfully loaded SciterAPI()!\n";

		//
		// CREATE SCITER WINDOW
		//

		//create an embedded sciter window
		window = SciterCreateWindow(
			SW_CHILD, 
			nullptr, 
			nullptr, 
			nullptr, 
			nullptr);
		if (!window)
		{
			cout << "Error: Failed to create Sciter window!\n";
			return;
		}
		else cout << "Successfully created Sciter window!\n";

		//remove title bar and borders
		LONG style = GetWindowLong(window, GWL_STYLE);
		style &= ~(
			WS_CAPTION
			| WS_MAXIMIZEBOX
			| WS_SIZEBOX
			| WS_THICKFRAME
			| WS_POPUP);
		SetWindowLong(window, GWL_STYLE, style);

		//remove extra styles
		LONG exStyle = GetWindowLong(window, GWL_EXSTYLE);
		exStyle &= ~(
			WS_EX_DLGMODALFRAME
			| WS_EX_WINDOWEDGE);
		SetWindowLong(window, GWL_EXSTYLE, exStyle);

		//set parent window
		HWND glfwWindow = glfwGetWin32Window(Render::window);
		SetParent(window, glfwWindow);

		//ensure Sciter is visible and always on top
		ShowWindow(window, SW_SHOW);

		//
		// LOAD HOMEPAGE
		//

		//load an HTML file
		string HTMLFilePath = (path(Browser::filesPath) / "htmlTest.html").string();
		if (!exists(HTMLFilePath))
		{
			cout << "Error: Failed to find HTML text file at '" << HTMLFilePath << "'!\n";
			return;
		}
		else cout << "Successfully found HTML test file!\n";
		wstring wideHTMLFilePath(HTMLFilePath.begin(), HTMLFilePath.end());
		LoadHTMLFromFile(wideHTMLFilePath.c_str());
	}

	void Content::LoadHTMLFromFile(const wchar_t* path)
	{
		int sizeNeeded = WideCharToMultiByte(
			CP_UTF8,
			0,
			path,
			-1,
			nullptr,
			0,
			nullptr,
			nullptr);

		string pathStr(sizeNeeded, 0);

		WideCharToMultiByte(
			CP_UTF8,
			0,
			path,
			-1,
			&pathStr[0],
			sizeNeeded,
			nullptr,
			nullptr);

		pathStr.pop_back();

		if (!SciterLoadFile(window, path))
		{
			cout << "Failed to load Sciter UI: '" << pathStr << "'!\n";
		}
		else
		{
			cout << "Sciter UI loaded: '" << pathStr << "'!\n";
		}
	}

	void Content::LoadHTMLFromMemory(const string& html)
	{
		//convert the string to Sciter-compatible format
		LPCBYTE htmlData = (LPCBYTE)html.c_str();
		UINT dataLength = (UINT)html.length();

		if (!SciterLoadHtml(
			window,
			htmlData,
			dataLength,
			NULL))
		{
			cout << "Failed to load Sciter UI!\n";
		}
		else
		{
			cout << "Successfully loaded Sciter UI!\n";
		}
	}

	vec4 Content::GetPosAndSize()
	{
		int fbWidth, fbHeight;
		glfwGetFramebufferSize(Render::window, &fbWidth, &fbHeight);

		int posX = 0;
		int posY = 110; //offset to avoid overlapping top imgui window
		int sizeX = fbWidth;
		int sizeY = fbHeight - 110; //Sciter fills the remaining area

		return vec4(
			posX,
			posY,
			sizeX,
			sizeY);
	}

	void Content::UpdateContent(const vec4& posAndSize)
	{
		if (Content::window)
		{
			int posX = posAndSize.x;
			int posY = posAndSize.y;
			int sizeX = posAndSize.z;
			int sizeY = posAndSize.w;

			//resize Sciter window to match ImGui
			SetWindowPos(
				Content::window,
				HWND_TOPMOST,
				posX,
				posY,
				sizeX,
				sizeY,
				SWP_NOZORDER
				| SWP_SHOWWINDOW);
		}
	}

	void Content::ForceRedraw(const vec4& posAndSize)
	{
		int sizeX = posAndSize.z;
		int sizeY = posAndSize.w;

		//force Sciter to refresh its layout
		SendMessage(
			Content::window,
			WM_SIZE,
			0,
			MAKELPARAM(sizeX, sizeY));

		//force Sciter redraw
		RedrawWindow(
			Content::window,
			NULL,
			NULL,
			RDW_INVALIDATE
			| RDW_UPDATENOW);
	}
}