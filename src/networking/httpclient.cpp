//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

#include <iostream>
#include <filesystem>

//external
#include "curl.h"

//browser
#include "httpclient.hpp"
#include "core.hpp"

using Core::Browser;

using std::cout;
using std::filesystem::path;

namespace Networking
{
	HTTPResponse HTTPClient::SendRequest(const HTTPRequest& request)
	{
		CURL* curl = curl_easy_init();
		if (!curl)
		{
			cout << "Error: Failed to initialize LibCurl!\n";
			return HTTPResponse(500, "Internal Error");
		}

		string curlCertPath = (path(Browser::filesPath) / "certifications" / "cacert.pem").string();

		string responseData;
		curl_easy_setopt(curl, CURLOPT_URL, request.GetURL().c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		//assign curl certification path
		curl_easy_setopt(curl, CURLOPT_CAINFO, curlCertPath.c_str());

		//let OpenSSL auto-negotiate TLS version
		curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_MAX_DEFAULT);

		CURLcode res = curl_easy_perform(curl);
		long responseCode = 0;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);

		curl_easy_cleanup(curl);

		return HTTPResponse(responseCode, responseData);
	}

	size_t HTTPClient::WriteCallback(
		void* contents, 
		size_t size, 
		size_t nmemb, 
		string* userData)
	{
		userData->append((char*)contents, size * nmemb);
		return size * nmemb;
	}
}