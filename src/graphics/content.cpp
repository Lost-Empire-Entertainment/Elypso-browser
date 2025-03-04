//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <iostream>
#include <string>
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
using std::string;
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
			| WS_THICKFRAME);
		style |= WS_POPUP;
		SetWindowLong(window, GWL_STYLE, style);

		//remove extra styles
		LONG exStyle = GetWindowLong(window, GWL_EXSTYLE);
		exStyle &= ~WS_EX_DLGMODALFRAME;
		SetWindowLong(window, GWL_EXSTYLE, exStyle);

		//prevent user resizing from system events
		SetWindowLongPtr(
			window,
			GWL_STYLE,
			GetWindowLongPtr(window, GWL_STYLE)
			& ~WS_SIZEBOX);
		SetWindowLongPtr(
			window,
			GWL_EXSTYLE,
			GetWindowLongPtr(window, GWL_EXSTYLE)
			& ~WS_EX_WINDOWEDGE);

		//set parent window
		HWND glfwWindow = glfwGetWin32Window(Render::window);
		SetParent(window, glfwWindow);

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
		SciterLoadFile(window, wideHTMLFilePath.c_str());

		//
		// FINAL TWEAKS
		//

		//ensure Sciter is visible and always on top
		ShowWindow(window, SW_SHOW);
		RedrawWindow(
			window, 
			NULL, 
			NULL, 
			RDW_INVALIDATE 
			| RDW_UPDATENOW);

		//bring Sciter to front but below full-screen apps
		SetWindowPos(window, HWND_TOP, 0, 0, 800, 600, SWP_SHOWWINDOW);
	}

	void Content::LoadContent(const wchar_t* path)
	{
		/*
		if (!SciterLoadFile(hWnd, path))
		{
			cout << "Failed to load Sciter UI: '" << path << "'!\n";
		}
		else
		{
			cout << "Sciter UI loaded: '" << path << "'!\n";
		}
		*/
	}

	void Content::UpdateContent()
	{

	}
}