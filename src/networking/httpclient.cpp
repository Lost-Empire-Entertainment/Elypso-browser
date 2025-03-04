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

		//add User-Agent
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 11.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/122.0.0.0 Safari/537.36 Edg/122.0.0.0");

		//add Referrer
		curl_easy_setopt(curl, CURLOPT_REFERER, "https://www.google.com/");

		//enable cookies for session handling
		curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies.txt");
		curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt");

		//add custom headers
		struct curl_slist* headers = NULL;
		headers = curl_slist_append(headers, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8");
		headers = curl_slist_append(headers, "Accept-Language: en-US,en;q=0.9");
		headers = curl_slist_append(headers, "Connection: keep-alive");
		headers = curl_slist_append(headers, "Upgrade-Insecure-Requests: 1");

		if (headers) curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		//let Curl automatically decompress responses
		curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");

		//set timeout values
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);

		CURLcode res = curl_easy_perform(curl);
		long responseCode = 0;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);

		if (headers) curl_slist_free_all(headers);
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