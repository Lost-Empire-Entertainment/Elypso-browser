//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#include <string>

#include "glm.hpp"

//external
#include "sciter-x.h"

namespace Graphics
{
	using std::string;
	using glm::vec4;

	class Content
	{
	public:
		/// <summary>
		/// The Sciter window
		/// </summary>
		static inline HWINDOW window;

		/// <summary>
		/// Initialize Sciter
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Load directly from an html file
		/// </summary>
		static void LoadHTMLFromFile(const wchar_t* path);
		/// <summary>
		/// Load from memory
		/// </summary>
		static void LoadHTMLFromMemory(const string& html);

		/// <summary>
		/// Returns the position and scale Sciter window should use
		/// </summary>
		static vec4 GetPosAndSize();

		/// <summary>
		/// Regular Sciter window position and scale update
		/// </summary>
		static void UpdateContent(const vec4& posAndSize);

		/// <summary>
		/// Force-redraws Sciter window content, only necessary in rare cases.
		/// </summary>
		static void ForceRedraw(const vec4& posAndSize);
	};
}