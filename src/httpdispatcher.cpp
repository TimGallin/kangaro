#include "kangaro.h"
#include "httpdispatcher.h"


namespace kangaro{

	HttpDispatcher::HttpDispatcher(){

	}

	HttpDispatcher::~HttpDispatcher(){
		ReleaseResources();
	}

	http_dlib_enter_point HttpDispatcher::SelectFunctor(request_conf* request){
		if (request == NULL){
			return NULL;
		}

		if (_functor_loaded.find(request->path) == _functor_loaded.end()){
			//This requestion has been configured rightly,now check it if the matched dll loadded.
			if (_dll_loaded.find(request->lib_path) == _dll_loaded.end()){
				//If not Loaded,then try it.
				kangaro_module h = kangaro_os::load_lib(request->lib_path.c_str());
				if (h == 0) {
					LOG(ERROR) << "load lib failed. lib: " << request->lib_path << ".error: " << kangaro_errno;
					//Load fialed.
					return NULL;
				}

				_dll_loaded.insert(std::make_pair(request->lib_path, h));

				http_dlib_enter_point ep = (http_dlib_enter_point)kangaro_os::get_lib_funtion(h, request->enter_point.c_str());
				if (ep == NULL) {
					LOG(ERROR) << "GetProcAddr() failed.lib:" << request->lib_path << ".Procname " << request->enter_point;
					return NULL;
				}

				_functor_loaded.insert(std::make_pair(request->path, ep));
				return ep;

			}
			else{
				//The matched dll has been loaded,then get the configured function and add it to the functor-map.
				http_dlib_enter_point ep = (http_dlib_enter_point)kangaro_os::get_lib_funtion(_dll_loaded[request->lib_path], request->enter_point.c_str());
				if (ep == NULL) {
					return NULL;
				}
				_functor_loaded.insert(std::make_pair(request->path, ep));
				LOG(ERROR) << "GetProcAddr() failed.lib:" << request->lib_path << ".Procname " << request->enter_point;
				return ep;
			}

		}
		else{
			//Loaded
			return _functor_loaded[request->path];
		}

		return NULL;
	}

	void HttpDispatcher::ReleaseResources(){
		dll_res::const_iterator ite = _dll_loaded.cbegin();

		for (ite; ite != _dll_loaded.cend(); ++ite){
			kangaro_os::free_lib(ite->second);
		}
	}
}