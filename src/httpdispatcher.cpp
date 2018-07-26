#include "kangaro.h"
#include "httpdispatcher.h"


namespace kangaro{

	HttpDispatcher::HttpDispatcher(){

	}

	HttpDispatcher::~HttpDispatcher(){
		ReleaseResources();
	}

	HttpDispatcher::request_res HttpDispatcher::SelectFunctor(request_conf* request){
		request_res query;
		query.func_release = NULL;
		query.func_request = NULL;

		if (request == NULL){
			return query;
		}

		std::map<std::string, dll_res>::iterator lib_ite = _dlls_loaded.find(request->lib_path);
		if (lib_ite == _dlls_loaded.end()){
			//If not Loaded,then try it.
			if (AddDllRes(request->lib_path) == NULL){
				return query;
			}
		}

		//release
		query.func_release = lib_ite->second.func_release;

		funciton_res::iterator request_ite = lib_ite->second.func_request.find(request->path);
		
		//query
		if (request_ite == lib_ite->second.func_request.end()){
			query.func_request = (http_dlib_enter_point)kangaro_os::get_lib_funtion(lib_ite->second.hmod, request->enter_point.c_str());
			if (query.func_request == NULL) {
				LOG(ERROR) << "GetProcAddr() failed.lib:" << request->lib_path << ".Procname " << request->enter_point;
				return query;
			}
		}
		else
		{
			query.func_request = request_ite->second;
		}

		return query;
	}

	kangaro_module HttpDispatcher::AddDllRes(const std::string& dllpath){
		if (_dlls_loaded.find(dllpath) == _dlls_loaded.end()){
			dll_res dll_loading;
			//If not Loaded,then try it.
			kangaro_module h = kangaro_os::load_lib(dllpath.c_str());
			if (h == 0) {
				LOG(ERROR) << "load lib failed. lib: " << dllpath << ".error: " << kangaro_errno;
				//Load fialed.
				return NULL;
			}

			dll_loading.hmod = h;
			dll_loading.func_release = (http_dlib_release)kangaro_os::get_lib_funtion(h, KANGARO_DLL_RELEASE_RES_FUNCNAME);

			_dlls_loaded.insert(std::make_pair(dllpath, dll_loading));

			return h;
		}
	}

	void HttpDispatcher::ReleaseResources(){
		std::map<std::string, dll_res>::const_iterator ite = _dlls_loaded.cbegin();

		for (ite; ite != _dlls_loaded.cend(); ++ite){
			kangaro_os::free_lib(ite->second.hmod);
		}
	}
}