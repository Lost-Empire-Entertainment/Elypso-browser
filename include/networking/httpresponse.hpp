//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

#include <string>

namespace Networking
{
	using std::string;

	class HTTPResponse
	{
	public:
		HTTPResponse(int statusCode, const string& body);

		int GetStatusCode() const;
		string GetBody() const;
	private:
		int statusCode;
		string body;
	};
}