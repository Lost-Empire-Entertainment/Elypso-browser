//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#include <string>
#include <unordered_map>

namespace Networking
{
	using std::string;
	using std::unordered_map;

	class NetworkManager
	{
	public:
		static bool HasInternet();

		static void ParseURL(const string& URL);
	private:
		static inline unordered_map<string, string> localFiles;

		static void FillLocalFilePaths();

		static void ParseLocalFileURL(const string& URL);
		static void ParseWebsiteURL(const string& URL);
	};
}