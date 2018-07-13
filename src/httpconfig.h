#ifndef KANGA_HTTPSERVER_CONFIG_HEAD
#define KANGA_HTTPERVER_CONFIG_HEAD

#pragma once

#include <string>

namespace kangaro{
	//
	//Reqesut Config
	//
	typedef struct _request_conf_{
		//Dynamic lib(0) OR class(1).
		int type;

		//Dynamic lib path.
		std::string lib_path;

		//Request path.
		std::string path;

		//Function of dynamic lib.
		std::string enter_point;

		//Next config
		_request_conf_* next;
	}request_conf;


	class HttpServerConfig{
	public:
		HttpServerConfig();
		~HttpServerConfig();

		static HttpServerConfig* GetInstance();

		/*
		 * Description:
		 * 		Load config file,read it into memory.
		 */
		int Load(const char* pConfigFile);

		std::string GetHTTPPort() const;

		/*
		Description:
		Select a request-config via path.
		*/
		request_conf* SelectRequestConfig(const std::string& name);
	private:
		/*
		Free all config struct.
		*/
		void FreeAllConfig();

		std::string _http_port;

		int _back_log;

		request_conf* _first_request;
	};

}
#endif
