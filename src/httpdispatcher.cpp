#include "kangaro.h"
#include "httpdispatcher.h"


namespace kangaro{

	HttpDispatcher::HttpDispatcher(){

	}

	HttpDispatcher::~HttpDispatcher(){

	}

	http_dlib_enter_point HttpDispatcher::SelectFunctor(const std::string& request_path){

		if (_functor_loaded.find(request_path) == _functor_loaded.end()){
			//NoT GetProcAddr
			request_conf* req = HttpServerConfig::GetInstance()->SelectRequestConfig(request_path);
			if (req == NULL){
				return NULL;
			}

			if (_dll_loaded.find(req->lib_path) == _dll_loaded.end()){
				//Not Loaded

			}
			else{
				http_dlib_enter_point d = (http_dlib_enter_point)kangaro_soc::get_lib_funtion(_dll_loaded[req->lib_path], request_path.c_str());
				_functor_loaded.insert(std::make_pair(request_path, d));
			}

		}
		else{
			//Loaded
			return _functor_loaded[request_path];
		}
	}
}