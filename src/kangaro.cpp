#include "kangaro.h"
#include "httpconfig.h"
#include "pouchserver.h"

int main(int argc, char* argv[]){
	//kangaro_module h = kangaro_os::load_lib("E:\\code\\kangaro_test\\Debug\\kangaro_test.dll");

	//kangaro_os::get_lib_funtion(h, "request_test");


	kangaro::HttpServerConfig config;
	if (config.Load("E:\\timgallin\\projects\\kangaro\\config\\httoconf.test.xml") != KANGA_OK){
		return 0;
	}

	kangaro::PouchSvr tm;
	tm.Init();
	tm.RunSvr();
	return 0;	
}
