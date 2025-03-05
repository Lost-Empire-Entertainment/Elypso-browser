//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#include <string>

//external
#include "imgui.h"

namespace GUI
{
	using std::string;

	class GUI_Browser
	{
	public:
		static void Initialize();

		static ImVec2 CenterWindow(const ImVec2& size);

		static void Render();
		static void Shutdown();
	private:
		static inline bool isImguiInitialized;

		static constexpr int bufferSize = 512;
		static inline char websiteChar[bufferSize];
		static inline string websiteString;

		static void SetStyle();

		static void RenderTopBar();
		static void RenderBrowserTopWindow();

		static void LoadPageInThread(const string& URL);
	};
}