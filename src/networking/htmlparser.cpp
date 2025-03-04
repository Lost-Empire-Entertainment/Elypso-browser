//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <iostream>

//browser
#include "htmlparser.hpp"
#include "content.hpp"

using Graphics::Content;

using std::cout;

namespace Networking
{
	void HTMLParser::ParseHTML(const string& html)
	{
		lxb_html_document_t* document = lxb_html_document_create();
		if (!document)
		{
			cout << "Error: Failed to create Lexbor HTML document!\n";
			return;
		}

		//parse html
		lxb_status_t status = lxb_html_document_parse(
			document, 
			(const lxb_char_t*)html.c_str(), 
			html.length());
		if (status != LXB_STATUS_OK)
		{
			cout << "Error: Failed to parse HTML!\n";
			lxb_html_document_destroy(document);
			return;
		}

		cout << "HTML parsed successfully!\n";

		//send the html data to the main thread
		string* htmlCopy = new string(html);
		PostMessage(
			Content::window, 
			WM_LOAD_HTML,
			0,
			reinterpret_cast<LPARAM>(htmlCopy));

		Content::LoadHTMLFromMemory(html);

		lxb_html_document_destroy(document);
	}
}