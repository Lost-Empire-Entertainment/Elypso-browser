//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <iostream>

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
		parsedElements.clear();

		//start extracting from <body>
		lxb_html_body_element_t* body_element = lxb_html_document_body_element(document);
		lxb_dom_element_t* body = lxb_dom_interface_element(body_element);
		if (body) ExtractText(lxb_dom_interface_node(body));

		lxb_html_document_destroy(document);

		cout << "Finished HTML head search.\n";
	}

	void HTMLParser::ExtractText(lxb_dom_node_t* root)
	{
		lxb_dom_node_t* node = root->first_child;

		while (node)
		{
			if (node->type == LXB_DOM_NODE_TYPE_ELEMENT)
			{
				lxb_dom_element_t* element = lxb_dom_interface_element(node);
				lxb_tag_id_t tag_id = lxb_dom_element_tag_id(element);

				//extract text content
				lxb_char_t* text = lxb_dom_node_text_content(node, NULL);
				if (text)
				{
					HTMLElement htmlElement;
					htmlElement.text = (char*)text;

					//store tag type for styling
					if (tag_id == LXB_TAG_H1) htmlElement.tag = "h1";
					else if (tag_id == LXB_TAG_H2) htmlElement.tag = "h2";
					else if (tag_id == LXB_TAG_P) htmlElement.tag = "p";
					else if (tag_id == LXB_TAG_A) htmlElement.tag = "a";

					if (!htmlElement.text.empty())
					{
						parsedElements.push_back(htmlElement);
					}
				}
			}
			ExtractText(node); //recursive call to find nested elements
			node = node->next;
		}
	}
}