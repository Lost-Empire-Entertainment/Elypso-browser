//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

//browser
#include "httprequest.hpp"

namespace Networking
{
	HTTPRequest::HTTPRequest(
		const string& protocol, 
		const string& host, 
		const string& path) :
		protocol(protocol),
		host(host),
		path(path) {}

	void HTTPRequest::SetMethod(const string& method)
	{
		this->method = method;
	}

	void HTTPRequest::AddHeader(const string& key, const string& value)
	{
		headers[key] = value;
	}

	void HTTPRequest::SetBody(const string& body)
	{
		this->body = body;
	}

	string HTTPRequest::GetURL() const
	{
		return protocol + "://" + host + path;
	}

	string HTTPRequest::GetMethod() const
	{
		return method;
	}

	unordered_map<string, string> HTTPRequest::GetHeaders() const
	{
		return headers;
	}

	string HTTPRequest::GetBody() const 
	{
		return body;
	}
}