//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

//Used for sending parsed HTML data from worker thread to main thread
#define WM_LOAD_HTML (WM_APP + 1)

//Used for sending parsed CSS data from worker thread to main thread
#define WM_LOAD_CSS (WM_APP + 1)

#pragma once

#include <string>

//external
#include "glm.hpp"
#include "sciter-x.h"

namespace Graphics
{
	using std::string;
	using glm::vec4;
	using std::wstring;

	class Content
	{
	public:
		/// <summary>
		/// The Sciter window
		/// </summary>
		static inline HWINDOW window;

		/// <summary>
		/// Original window procedure
		/// </summary>
		static inline WNDPROC originalWndProc;

		/// <summary>
		/// Initialize Sciter
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Load directly from an html file
		/// </summary>
		static void LoadHTMLFromFile(const string& path);
		/// <summary>
		/// Load from memory
		/// </summary>
		static void LoadHTMLFromMemory(const string& html);

		/// <summary>
		/// Regular Sciter window position and scale update
		/// </summary>
		static void UpdateContent();

		/// <summary>
		/// Force-redraws Sciter window content, only necessary in rare cases.
		/// </summary>
		static void ForceRedraw();
	private:
		/// <summary>
		/// Returns the position and scale Sciter window should use
		/// </summary>
		static vec4 GetPosAndSize();

		static void SubclassGLFWWindow();

		/// <summary>
		/// Handle loading websites from another thread
		/// </summary>
		static LRESULT CALLBACK WndProc(
			HWND hWnd, 
			UINT msg, 
			WPARAM wParam, 
			LPARAM lParam);

		/// <summary>
		/// Send Sciter debug messages to console
		/// </summary>
		static VOID SC_CALLBACK SciterDebugCallback(
			LPVOID param,
			UINT subSystem,
			UINT severity,
			LPCWSTR text,
			UINT textLength);
	};
}