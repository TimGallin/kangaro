#ifndef KANGA_HTTP_DISPATHER_HEAD
#define KANGA_HTTP_DISPATHER_HEAD

#pragma once
#include "kangaro.h"
#include "httpconfig.h"
#include <string>
#include <map>
#include <vector>

namespace kangaro{
	//Dynamic lib Enter-Point Function Type.
	typedef int(*http_dlib_enter_point) (HTTPMessage& _in, HTTPMessage& _out, SvrHandle& _handle);


	class HttpDispatcher{
	public:
		typedef std::map<std::string, http_dlib_enter_point> funciton_res;
		typedef std::map<std::string, kangaro_module> dll_res;

		HttpDispatcher();
		~HttpDispatcher();

		http_dlib_enter_point SelectFunctor(const std::string& request_path);

	private:
		/*
		<DLL-resource-path,load-module>
		*/
		dll_res _dll_loaded;

		/*
		<request-path,reflected-function>
		*/
		funciton_res _functor_loaded;
	};
}



#endif
