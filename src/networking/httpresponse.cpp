//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

//browser
#include "httpresponse.hpp"

namespace Networking
{
	HTTPResponse::HTTPResponse(
		int statusCode, 
		const string& body) :
		statusCode(statusCode),
		body(body) {}

	int HTTPResponse::GetStatusCode() const
	{
		return statusCode;
	}

	string HTTPResponse::GetBody() const
	{
		return body;
	}
}