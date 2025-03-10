//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#include <string>
#include <vector>

//external
#include "Lexbor/html/parser.h"

namespace Networking
{
	using std::string;
	using std::vector;

	class HTMLParser
	{
	public:
		struct HTMLElement
		{
			string tag;
			string text;
		};

		static void ParseHTML(const string& html);
	private:
		static void ExtractCSS(
			lxb_html_document_t* document, 
			vector<string>& cssData);
	};
}