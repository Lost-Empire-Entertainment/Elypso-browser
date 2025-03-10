//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <iostream>
#include <filesystem>
#include <locale>
#include <codecvt>
#include <regex>

//external
#include "glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw3native.h"

//browser
#include "content.hpp"
#include "core.hpp"
#include "render.hpp"
#include "gui.hpp"

using Core::Browser;
using GUI::GUI_Browser;

using std::cout;
using std::filesystem::path;
using std::filesystem::exists;
using std::wstring;
using std::regex;
using std::regex_replace;

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

		//displays sciter debug output in console
		SciterSetupDebugOutput(
			window, 
			nullptr, 
			&SciterDebugCallback);

		SubclassGLFWWindow();

		//
		// LOAD HOMEPAGE
		//

		string homeHTML = (path(Browser::filesPath) / "pages" / "home.html").string();
		LoadHTMLFromFile(homeHTML);
	}

	void Content::LoadHTMLFromFile(const string& path)
	{
		cout << "Started loading HTML from file...\n";

		if (!exists(path))
		{
			cout << "Error: Failed to find HTML file at '" << path << "'!\n";
			return;
		}
		else cout << "Successfully found HTML file! Starting to load...\n";

		wstring wideHTMLFilePath(path.begin(), path.end());
		if (!SciterLoadFile(window, wideHTMLFilePath.c_str()))
		{
			cout << "Error: Failed to load Sciter from HTML file: '" << path << "'!\n";
		}
		else cout << "Successfully loaded Sciter from HTML file!\n";

		GUI_Browser::SetSearchBarText(path);

		//force a redraw after loading a html page from a file
		UpdateContent();
	}

	void Content::LoadHTMLFromMemory(const string& html)
	{
		cout << "Started loading HTML from memory...\n";

		if (!window)
		{
			cout << "Error: Invalid Sciter window handle!\n";
			return;
		}

		if (html.empty())
		{
			cout << "Error: Empty HTML string!\n";
			return;
		}

		/*
		regex externalJSRegex(R"(<script\s+[^>]*src=["']([^"']+\.js)["'])", regex::icase);
		bool hasExternalJS = regex_search(html, externalJSRegex);
		if (hasExternalJS)
		{
			string jsHTML = (path(Browser::filesPath) / "pages" / "jsRequired.html").string();
			LoadHTMLFromFile(jsHTML);
		}
		else
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
				cout << "Error: Failed to load Sciter from memory!\n";
			}
			else cout << "Successfully loaded Sciter from memory!\n";
		}
		*/

		//convert the string to Sciter-compatible format
		LPCBYTE htmlData = (LPCBYTE)html.c_str();
		UINT dataLength = (UINT)html.length();

		if (!SciterLoadHtml(
			window,
			htmlData,
			dataLength,
			NULL))
		{
			cout << "Error: Failed to load Sciter from memory!\n";
		}
		else cout << "Successfully loaded Sciter from memory!\n";
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

	void Content::UpdateContent()
	{
		if (Content::window)
		{
			const vec4& posAndSize = GetPosAndSize();

			/*
			static int prevPosX = -1;
			static int prevPosY = -1;
			static int prevSizeX = -1;
			static int prevSizeY = -1;

			RECT rect{};
			GetWindowRect(Content::window, &rect);

			int rectPosX = rect.left;               //Current X position
			int rectPosY = rect.top;                //Current Y position
			int rectSizeX = rect.right - rect.left; //Current X size
			int rectSizeY = rect.bottom - rect.top; //Current Y size
			
			int posX = static_cast<int>(posAndSize.x + 0.5f);  //Target X position
			int posY = static_cast<int>(posAndSize.y + 0.5f);  //Target Y position
			int sizeX = static_cast<int>(posAndSize.z + 0.5f); //Target X size
			int sizeY = static_cast<int>(posAndSize.w + 0.5f); //Target Y size

			if (prevPosX != posX
				|| prevPosY != posY
				|| prevSizeX != sizeX
				|| prevSizeY != sizeY)
			{
				//resize Sciter window to match ImGui
				SetWindowPos(
					Content::window,
					HWND_TOPMOST,
					posX,
					posY,
					sizeX,
					sizeY,
					SWP_NOZORDER
					| SWP_NOACTIVATE
					| SWP_SHOWWINDOW);

				prevPosX = posX;
				prevPosY = posY;
				prevSizeX = sizeX;
				prevSizeY = sizeY;

				cout << "Sciter window was redrawn!\n";
			}
			*/

			int posX = static_cast<int>(posAndSize.x + 0.5f);  //Target X position
			int posY = static_cast<int>(posAndSize.y + 0.5f);  //Target Y position
			int sizeX = static_cast<int>(posAndSize.z + 0.5f); //Target X size
			int sizeY = static_cast<int>(posAndSize.w + 0.5f); //Target Y size

			//resize Sciter window to match ImGui
			SetWindowPos(
				Content::window,
				HWND_TOPMOST,
				posX,
				posY,
				sizeX,
				sizeY,
				SWP_NOZORDER
				| SWP_NOACTIVATE
				| SWP_SHOWWINDOW);
		}
	}

	void Content::ForceRedraw()
	{
		const vec4& posAndSize = GetPosAndSize();
		
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

	void Content::SubclassGLFWWindow()
	{
		HWND glfwHwnd = glfwGetWin32Window(Render::window);
		if (glfwHwnd)
		{
			originalWndProc = reinterpret_cast<WNDPROC>(
				SetWindowLongPtr(
				glfwHwnd,
				GWLP_WNDPROC,
				reinterpret_cast<LONG_PTR>(WndProc)));

			if (originalWndProc == nullptr)
			{
				cout << "Error: Failed to subclass window procedure!\n";
			}
		}
	}

	LRESULT CALLBACK Content::WndProc(
		HWND hWnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam)
	{
		if (msg == WM_LOAD_HTML)
		{
			string* html = reinterpret_cast<string*>(lParam);
			LoadHTMLFromMemory(*html);

			delete html;
			return 0;
		}
		else if (msg == WM_LOAD_CSS)
		{
			string* css = reinterpret_cast<string*>(lParam);
			SciterAppendMasterCSS(
				reinterpret_cast<LPCBYTE>(css->c_str()), 
				css->length());

			delete css;
			return 0;
		}
		return CallWindowProc(
			originalWndProc, 
			hWnd, 
			msg,
			wParam, 
			lParam);
	}

	VOID SC_CALLBACK Content::SciterDebugCallback(
		LPVOID param,
		UINT subSystem,
		UINT severity,
		LPCWSTR text,
		UINT textLength)
	{
		wstring wtext(text, textLength);
		
		int sizeRequired = WideCharToMultiByte(
			CP_UTF8,
			0,
			wtext.data(),
			static_cast<int>(wtext.size()),
			nullptr,
			0,
			nullptr,
			nullptr);

		string message(sizeRequired, 0);
		WideCharToMultiByte(
			CP_UTF8,
			0,
			wtext.data(),
			static_cast<int>(wtext.size()),
			&message[0],
			sizeRequired,
			nullptr,
			nullptr);

		string severityString{};
		switch (severity)
		{
		case 0:
			severityString = "INFO";
			break;
		case 1:
			severityString = "WARNING";
			break;
		case 2:
			severityString = "ERROR";
			break;
		default:
			severityString = "UNKNOWN";
			break;
		}

		cout << "[" << severityString << "] [SCITER] " << message;
	}
}