//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

//external
#include "sciter-x.h"

namespace Graphics
{
	class Content
	{
	public:
		static inline HWINDOW window;

		static void Initialize();

		static void LoadContent(const wchar_t* path);

		/// <summary>
		/// Regular Sciter window position and scale update
		/// </summary>
		static void UpdateContent(int sizeX, int sizeY);

		/// <summary>
		/// Force-redraws Sciter window content, only necessary in rare cases.
		/// </summary>
		static void ForceRedraw(int sizeX, int sizeY);
	};
}