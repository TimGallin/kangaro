#include "kangaro.h"
#include "httpdispatcher.h"


namespace kangaro{

	HttpDispatcher::HttpDispatcher(){

	}

	HttpDispatcher::~HttpDispatcher(){

	}

	http_dlib_enter_point HttpDispatcher::SelectFunctor(const std::string& request_path){

		if (_functor_loaded.find(request_path) == _functor_loaded.end()){
			//If cannot find the matched functor,then query the xml configuration.
			request_conf* req = HttpServerConfig::GetInstance()->SelectRequestConfig(request_path);
			if (req == NULL){
				//If this request has no config in xml,then return null.
				return NULL;
			}

			//This requestion has been configured rightly,now check it if the matched dll loadded.
			if (_dll_loaded.find(req->lib_path) == _dll_loaded.end()){
				//If not Loaded,then try it.
				kangaro_module h = kangaro_os::load_lib(req->lib_path.c_str());
				if (h == 0) {
					//Load fialed.
					return NULL;
				}

				_dll_loaded.insert(std::make_pair(req->lib_path, h));

				http_dlib_enter_point ep = (http_dlib_enter_point)kangaro_os::get_lib_funtion(h, req->enter_point.c_str());
				if (ep == NULL) {
					return NULL;
				}

				_functor_loaded.insert(std::make_pair(request_path, ep));
				return ep;

			}
			else{
				//The matched dll has been loaded,then get the configured function and add it to the functor-map.
				http_dlib_enter_point ep = (http_dlib_enter_point)kangaro_os::get_lib_funtion(_dll_loaded[req->lib_path], req->enter_point.c_str());
				if (ep == NULL) {
					return NULL;
				}
				_functor_loaded.insert(std::make_pair(request_path, ep));
				return ep;
			}

		}
		else{
			//Loaded
			return _functor_loaded[request_path];
		}
	}
}