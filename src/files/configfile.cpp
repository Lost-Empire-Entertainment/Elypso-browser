//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <fstream>
#include <iostream>
#include <filesystem>
#include <map>

//external
#include "imgui.h"

//browser
#include "configfile.hpp"
#include "stringutils.hpp"
#include "fileutils.hpp"
#include "core.hpp"

using Utils::String;
using Utils::File;
using Core::Browser;

using std::ifstream;
using std::ofstream;
using std::cout;
using std::to_string;
using std::filesystem::exists;
using std::filesystem::path;
using std::filesystem::current_path;
using std::map;
using std::ios;

namespace Files
{
	void ConfigFile::LoadConfigFile()
	{
		if (configFilePath == "") configFilePath = (path(Browser::docsPath) / "config.txt").string();

		if (!exists(configFilePath)) CreateNewConfigFile();
		else
		{
			//opens file in binary mode and moves to end
			//if file is empty then a new one is created
			ifstream configFileBinary(configFilePath, ios::binary | ios::ate);
			if (configFileBinary.tellg() == 0)
			{
				configFileBinary.close();

				cout << "Failed to load config file '" << configFilePath << "' because it was empty! Creating a new one.\n\n";

				CreateNewConfigFile();
				return;
			}

			ifstream configFile(configFilePath);
			if (!configFile.is_open())
			{
				cout << "Error: Failed to open config file '" << configFilePath << "'!\n\n";
				return;
			}

			keys.clear();
			values.clear();

			string line;
			map<string, string> configValue;
			while (getline(configFile, line))
			{
				if (!line.empty()
					&& line.find("=") != string::npos)
				{
					vector<string> splitLine = String::Split(line, '=');
					string key = splitLine[0];
					string value = splitLine[1];

					//remove one space in front of value if it exists
					if (value[0] == ' ') value.erase(0, 1);
					//remove one space in front of each value comma if it exists
					for (size_t i = 0; i < value.length(); i++)
					{
						if (value[i] == ','
							&& i + 1 < value.length()
							&& value[i + 1] == ' ')
						{
							value.erase(i + 1, 1);
						}
					}

					keys.push_back(key);
					values.push_back(value);
				}
			}

			configFile.close();

			cout << "Successfully loaded config file!\n";
		}
	}

	void ConfigFile::SaveConfigFile()
	{
		if (exists(configFilePath))
		{
			File::DeleteFileOrfolder(configFilePath);
		}

		ofstream configFile(configFilePath);

		if (!configFile.is_open())
		{
			cout << "Error: Couldn't write into config file '" << configFilePath << "'!\n";
			return;
		}

		for (int i = 0; i < keys.size(); i++)
		{
			string key = keys[i];
			string value = values[i];

			configFile << key << "= " << value << "\n";
		}

		configFile.close();

		cout << "\nSuccessfully saved config file!\n";
	}

	string ConfigFile::GetValue(const string& key, bool silent)
	{
		int keyIndex = -1;
		for (int i = 0; i < keys.size(); i++)
		{
			if (keys[i] == key)
			{
				keyIndex = i;
				break;
			}
		}

		if (keyIndex != -1)
		{
			return values[keyIndex];
		}
		else
		{
			if (!silent)
			{
				cout << "Error: Cannot get config key " << key << " value because it does not exist!\n";
			}
			return "";
		}
	}

	void ConfigFile::SetValue(const string& key, const string& value)
	{
		int keyIndex = -1;
		for (int i = 0; i < keys.size(); i++)
		{
			if (keys[i] == key)
			{
				keyIndex = i;
				break;
			}
		}

		if (keyIndex != -1)
		{
			values[keyIndex] = value;

			if (key == "gui_fontScale")
			{
				ImGui::GetIO().FontGlobalScale = stof(value);
			}
		}
		else
		{
			cout << "Error: Cannot set value to config key " << key << " because it does not exist!\n";
		}
	}

	void ConfigFile::CreateNewConfigFile()
	{
		if (configFilePath == "") configFilePath = (path(Browser::docsPath) / "config.txt").string();

		keys.clear();
		values.clear();

		keys.push_back("gui_fontScale");
		values.push_back("1.5");

		ofstream configFile(configFilePath);

		if (!configFile.is_open())
		{
			cout << "Error: Couldn't write into config file!\n";
			return;
		}

		for (int i = 0; i < keys.size(); i++)
		{
			string key = keys[i];
			string value = values[i];

			configFile << key << "= " << value << "\n";
		}

		configFile.close();

		LoadConfigFile();
	}
}