//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <iostream>
#include <string>
#include <filesystem>

//external
#include "stb_image.h"
#include "glad.h"

//browser
#include "render.hpp"
#include "core.hpp"
#include "input.hpp"
#include "gui.hpp"

using Core::Browser;
using Core::Input;
using GUI::GUI_Browser;

using std::cout;
using std::string;
using std::filesystem::current_path;
using std::filesystem::path;

namespace Graphics
{
	void Render::Initialize()
	{
		//
		// GLFW SETUP
		//

		cout << "Initializing GLFW...\n";

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		cout << "GLFW initialized successfully!\n\n";

		//
		// WINDOW SETUP
		//

		cout << "Creating window...\n";

		//create a window object holding all the windowing data
		string windowName = Browser::name + " " + Browser::version;
		window = glfwCreateWindow(
			1280,
			720,
			(windowName).c_str(),
			NULL,
			NULL);

		if (window == NULL)
		{
			cout << "Error: Failed to create GLFW window!\n\n";
			return;
		}

		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, UpdateAfterRescale);
		glfwSetWindowSizeLimits(window, 800, 600, 7680, 4320);
		glfwSwapInterval(0);

		int width, height, channels;
		string iconpath = (path(Browser::filesPath) / "icon.png").string();
		unsigned char* iconData = stbi_load(iconpath.c_str(), &width, &height, &channels, 4);

		GLFWimage icon{};
		icon.width = width;
		icon.height = height;
		icon.pixels = iconData;

		glfwSetWindowIcon(window, 1, &icon);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		glfwSetMouseButtonCallback(window, Input::MouseButtonCallback);
		glfwSetScrollCallback(window, Input::ScrollCallback);
		glfwSetKeyCallback(window, Input::KeyCallback);
		glfwSetCursorPosCallback(window, Input::MouseMovementCallback);

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window) { Browser::Shutdown(); });

		cout << "Window initialized successfully!\n\n";

		//
		// GLAD SETUP
		//

		cout << "Initializing GLAD...\n";

		//check if glad is initialized before continuing
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			cout << "Error: Failed to initialize GLAD!\n\n";
			return;
		}

		cout << "GLAD initialized successfully!\n\n";

		GUI_Browser::Initialize();
	}

	void Render::UpdateAfterRescale(GLFWwindow* window, int width, int height)
	{
		//Calculate the new aspect ratio
		aspectRatio = static_cast<float>(width) / static_cast<float>(height);

		//Set the viewport based on the aspect ratio
		glViewport(0, 0, width, height);

		currentWidth = width;
		currentHeight = height;
	}

	void Render::WindowLoop()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		GUI_Browser::Render();

		//swap the front and back buffers
		glfwSwapBuffers(window);
		if (!Browser::IsUserIdle())
		{
			glfwPollEvents();
		}
		else glfwWaitEvents();
	}
}