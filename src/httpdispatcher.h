#ifndef KANGA_HTTP_DISPATHER_HEAD
#define KANGA_HTTP_DISPATHER_HEAD

#pragma once
#include "kangaro.h"
#include "httpconfig.h"
#include "httpparams.h"

#include <string>
#include <map>
#include <vector>

namespace kangaro{

	class HttpDispatcher{
	public:
		typedef struct _request_res_{
			http_dlib_enter_point func_request;
			http_dlib_release func_release;
		}request_res;

		HttpDispatcher();
		~HttpDispatcher();

		request_res SelectFunctor(request_conf* request);

		/*
		Add dll  to dll_res
		*/
		kangaro_module AddDllRes(const std::string& dllpath);
	private:
		/*
		FREE LIB
		*/
		void ReleaseResources();

		typedef std::map<std::string, http_dlib_enter_point> funciton_res;

		typedef struct _dll_res{
			kangaro_module hmod;
			funciton_res func_request;
			http_dlib_release func_release;
		}dll_res;

		/*
		<DLL-resource-path,load-module>
		*/
		std::map<std::string, dll_res> _dlls_loaded;

	};
}



#endif
