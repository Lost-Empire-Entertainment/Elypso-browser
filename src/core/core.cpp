//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#ifdef _WIN32
#include <Windows.h>
#include <ShlObj.h>
#include <TlHelp32.h>
#elif __linux__
#include <unistd.h>
#endif
#include <iostream>
#include <filesystem>
#include <fstream>

//external
#include "glfw3.h"

//browser
#include "core.hpp"
#include "configfile.hpp"
#include "gui.hpp"
#include "render.hpp"

using Files::ConfigFile;
using Graphics::Render;
using GUI::GUI_Browser;

using std::cout;
using std::wstring;
using std::filesystem::exists;
using std::filesystem::create_directory;
using std::filesystem::path;
using std::filesystem::current_path;
using std::ofstream;

namespace Core
{
	void Browser::Initialize()
	{
		name = "Elypso browser";
		version = "0.0.1";

		if (IsThisProcessAlreadyRunning(name))
		{
			CreateErrorPopup((name + " is already running!").c_str());
		}

		cout << "\n==================================================\n"
			<< "\n"
			<< "ENTERED ELYPSO BROWSER\n"
			<< "\n"
			<< "==================================================\n"
			<< ".\n"
			<< ".\n"
			<< ".\n\n";

		cout << "Initializing Elypso Browser...\n";

		//
		// SET FILES PATH
		//

		filesPath = (current_path() / "files").string();
		if (!exists(filesPath))
		{
			CreateErrorPopup("Couldn't find files folder!");
			return;
		}

		cout << name << " files path: " << filesPath << "\n\n";

		//
		// SET DOCUMENTS PATH
		//

#ifdef _WIN32
		PWSTR pwstrPath;
		HRESULT result = SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &pwstrPath);
		if (SUCCEEDED(result))
		{
			wstring wPath(pwstrPath);
			CoTaskMemFree(pwstrPath); //free the allocated memory

			//get the required buffer size
			int size_needed = WideCharToMultiByte(
				CP_UTF8,
				0,
				wPath.c_str(),
				static_cast<int>(wPath.length()),
				NULL,
				0,
				NULL,
				NULL);

			//convert wide string to utf-8 encoded narrow string
			string narrowPath(size_needed, 0);
			WideCharToMultiByte(
				CP_UTF8,
				0,
				wPath.c_str(),
				static_cast<int>(wPath.length()),
				&narrowPath[0],
				size_needed,
				NULL,
				NULL);

			size_t pos = 0;
			docsPath = (path(narrowPath) / "Elypso browser").string();
		}

		if (!exists(docsPath)) create_directory(docsPath);
#elif __linux__
		const char* homeDir = getenv("HOME");
		cout << "home dir: " << homeDir << "\n";
		if (!homeDir)
		{
			CreateErrorPopup("HOME environment is not set!");
		}

		string documentsFolder = path(homeDir) / "Documents";
		if (!exists(documentsFolder)) create_directory(documentsFolder);

		docsPath = path(documentsFolder) / "Elypso browser";
		if (!exists(docsPath)) create_directory(docsPath);
		cout << "docs path: " << docsPath << "\n";
#endif

		configFilePath = (path(docsPath) / "config.txt").string();
		cout << "config file path: " << configFilePath << "\n";
		if (!exists(configFilePath))
		{
			ofstream configFile(configFilePath);
			if (!configFile.is_open())
			{
				CreateErrorPopup("Failed to create config file.");
			}
			configFile.close();
		}
		ConfigFile::LoadConfigFile();

		Render::Initialize();
	}

	void Browser::CreateErrorPopup(const char* errorMessage)
	{
		string title = name + " has shut down";

		cout << "\n"
			<< "===================="
			<< "\n"
			<< "BROWSER SHUTDOWN\n"
			<< "\n\n"
			<< errorMessage
			<< "\n"
			<< "===================="
			<< "\n";

#ifdef _WIN32
		int result = MessageBoxA(nullptr, errorMessage, title.c_str(), MB_ICONERROR | MB_OK);

		if (result == IDOK) Shutdown(true);
#elif __linux__
		string command = "zenity --error --text=\"" + (string)errorMessage + "\" --title=\"" + title + "\"";
		int result = system(command.c_str());
		(void)result;
		Shutdown(true);
#endif
	}

	void Browser::CreateWarningPopup(const char* warningMessage)
	{
		string title = "Elypso Engine Warning";

		cout << "\n"
			<< "===================="
			<< "\n"
			<< "BROWSER WARNING"
			<< "\n\n"
			<< warningMessage
			<< "\n"
			<< "===================="
			<< "\n";

#ifdef _WIN32
		MessageBoxA(nullptr, warningMessage, title.c_str(), MB_ICONWARNING | MB_OK);
#elif __linux__
		string command = "zenity --warning --text=\"" + (string)warningMessage + "\" --title=\"" + title + "\"";
		int result = system(command.c_str());
		(void)result;
#endif
	}

	bool Browser::IsThisProcessAlreadyRunning(const string& processName)
	{
#ifdef _WIN32
		bool processFound = false;
		HANDLE hProcessSnap;
		PROCESSENTRY32 pe32{};

		DWORD currentProcessId = GetCurrentProcessId();

		//take a snapshot of all processes
		hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hProcessSnap == INVALID_HANDLE_VALUE)
		{
			cout << "Error: CreateToolhelp32Snapshot failed!\n";
			return false;
		}

		pe32.dwSize = sizeof(PROCESSENTRY32);

		if (!Process32First(hProcessSnap, &pe32))
		{
			cout << "Error: Process32First failed!\n";
			CloseHandle(hProcessSnap);
			return false;
		}

		do
		{
			//compare the current process name with the one to check
			if (strcmp(pe32.szExeFile, processName.c_str()) == 0)
			{
				//check if this is not the current process
				if (pe32.th32ProcessID != currentProcessId)
				{
					processFound = true;
					break;
				}
			}
		} while (Process32Next(hProcessSnap, &pe32));

		CloseHandle(hProcessSnap);
		return processFound;
#elif __linux__
		//get the process id
		pid_t currentPID = getpid();

		//construct the command to find processes by name, excluding the currend PID
		string command = "pgrep -x \"" + processName + "\" | grep -v " + to_string(currentPID) + " > /dev/null";

		//execute the command and return the result
		return (system(command.c_str()) == 0);
#endif
		return false;
	}

	void Browser::Render()
	{
		cout << "Reached window loop successfully!\n\n";

		isBrowserRunning = true;

		while (isBrowserRunning)
		{
			Render::WindowLoop();

			if (glfwWindowShouldClose(Render::window))
			{
				isBrowserRunning = false;
			}
		}
	}

	//counts as idle if minimized
	bool Browser::IsUserIdle()
	{
		//checks if window is minimized
		int width, height;
		glfwGetWindowSize(Render::window, &width, &height);
		if (width == 0 || height == 0) return true;

		else return false;
	}

	void Browser::Shutdown(bool immediate)
	{
		if (immediate)
		{
			isBrowserRunning = false;

			GUI_Browser::Shutdown();

			glfwTerminate();

			quick_exit(EXIT_SUCCESS);
		}
		else
		{
			isBrowserRunning = false;

			ConfigFile::SaveConfigFile();

			cout << "Cleaning up resources...\n";

			GUI_Browser::Shutdown();

			cout << "Shutdown complete!\n";

			cout << "\n==================================================\n"
				<< "\n"
				<< "EXITED BROWSER\n"
				<< "\n"
				<< "==================================================\n"
				<< ".\n"
				<< ".\n"
				<< ".\n\n";

			glfwTerminate();

			exit(EXIT_SUCCESS);
		}
	}
}