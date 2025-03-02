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

	class HTTPRequest
	{
	public:
		HTTPRequest(
			const string& protocol, 
			const string& host, 
			const string& path);

		void SetMethod(const string& method);
		void AddHeader(const string& key, const string& value);
		void SetBody(const string& body);

		string GetURL() const;
		string GetMethod() const;
		unordered_map<string, string> GetHeaders() const;
		string GetBody() const;
	private:
		string protocol;
		string host;
		string path;
		string method = "GET";
		unordered_map<string, string> headers;
		string body;
	};
}