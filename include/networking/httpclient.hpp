//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#pragma once

//browser
#include "httprequest.hpp"
#include "httpresponse.hpp"

namespace Networking
{
	class HTTPClient
	{
	public:
		HTTPResponse SendRequest(const HTTPRequest& request);
	private:
		static size_t WriteCallback(
			void* contents, 
			size_t size, 
			size_t nmemb, 
			string* userData);
	};
}