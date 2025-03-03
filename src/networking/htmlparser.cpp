//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <iostream>

//external
#include "Lexbor/html/parser.h"

//browser
#include "htmlparser.hpp"

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

		lxb_html_head_element_t* head_element = lxb_html_document_head_element(document);
		lxb_dom_element_t* head = lxb_dom_interface_element(head_element);
		if (!head)
		{
			cout << "Error: No <head> found in document!\n";
			lxb_html_document_destroy(document);
			return;
		}

		//loop through <head> children to find <title>
		lxb_dom_node_t* node = lxb_dom_interface_node(head)->first_child;
		while (node)
		{
			//check if node is a <title> tag
			if (lxb_dom_interface_element(node) != nullptr)
			{
				//get tag ID and check if it's <title>
				if (lxb_dom_element_tag_id(lxb_dom_interface_element(node)) == LXB_TAG_TITLE)
				{
					//get the text content of <title>
					lxb_char_t* text = lxb_dom_node_text_content(node, NULL);
					if (text)
					{
						cout << "Page title: " << text << "\n";
					}
					break;
				}
			}
			node = node->next;
		}

		lxb_html_document_destroy(document);

		cout << "Finished HTML head search.\n";
	}
}