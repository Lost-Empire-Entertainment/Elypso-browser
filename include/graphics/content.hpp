//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

//external
#include "sciter-x.h"
#include "glfw3.h"

namespace Graphics
{
	class Content
	{
	public:
		static inline HWINDOW window;

		static void Initialize();

		static void ForwardGLFWEvents(
			GLFWwindow* glfwWindow,
			int key,
			int scanCode,
			int action,
			int mods);

		static void LoadContent(const wchar_t* path);
		static void UpdateContent();
	};
}