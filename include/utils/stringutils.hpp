//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#include <vector>
#include <string>

namespace Utils
{
	using std::vector;
	using std::string;

	class String
	{
	public:
		/// <summary>
		/// Replace a part of a string with something else.
		/// </summary>
		/// <param name="original">Full string</param>
		/// <param name="search">Part to search for.</param>
		/// <param name="replacement">Part to replace searched part with.</param>
		/// <returns></returns>
		static string StringReplace(const string& original, const string& search, const string& replacement);

		/// <summary>
		/// Replace a part of a char with something else.
		/// </summary>
		/// <param name="original"></param>
		/// <param name="search"></param>
		/// <param name="replacement"></param>
		/// <returns></returns>
		static string CharReplace(const string& original, const char& search, const char& replacement);

		/// <summary>
		/// Split a string in two from the delimiter.
		/// </summary>
		/// <param name="input">Full string.</param>
		/// <param name="delimiter">Which char is the splitter?</param>
		/// <returns></returns>
		static vector<string> Split(const string& input, char delimiter);

		/// <summary>
		/// Remove everything except the selected instances.
		/// </summary>
		/// <param name="originalVector">The original vector we are editing</param>
		/// <param name="instance">The part that should be kept.</param>
		/// <returns></returns>
		static vector<string> RemoveExcept(const vector<string>& originalVector, const string& instance);

		/// <summary>
		/// Find and remove all duplicates of already existing strings in the string vector.
		/// </summary>
		/// <param name="originalVector">The original vector we are editing.</param>
		/// <returns></returns>
		static vector<string> RemoveDuplicates(const vector<string>& originalVector);

		/// <summary>
		/// Check if the selected file contains the selected string.
		/// </summary>
		/// <param name="filePath">Where is the file located?</param>
		/// <param name="targetString">What is the string you are looking for?</param>
		/// <returns></returns>
		static bool ContainsString(const string& filePath, const string& targetString);

		static bool CanConvertStringToFloat(const string& value);

		static bool CanConvertStringToInt(const string& value);

		/// <summary>
		/// Check if the character is allowed in paths in Windows
		/// </summary>
		/// <param name="c"></param>
		/// <returns></returns>
		static bool IsValidSymbolInPath(const char& c);
	};
}