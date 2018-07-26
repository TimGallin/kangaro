#include "kangaro.h"
#include "httpconfig.h"
#include "pouchserver.h"
#include <iomanip>
#include <io.h>
#include <direct.h>

int main(int argc, char* argv[]){
	//Init GLog
	google::InitGoogleLogging(argv[0]);

	FLAGS_logbufsecs = 0;  //日志实时输出         
	FLAGS_max_log_size = 10; //最大日志文件大小   
	FLAGS_minloglevel = google::GLOG_INFO;

	//Set File
	if (_access("log", 0) == -1){
		_mkdir("log");
	}

	const char* logfile= "log\\";
	google::SetLogFilenameExtension(".txt");
	google::SetLogDestination(google::GLOG_INFO, logfile);
	google::SetLogDestination(google::GLOG_WARNING, logfile);
	google::SetLogDestination(google::GLOG_ERROR, logfile);

	//Load Config xml.
	kangaro::HttpServerConfig config;
	if (config.Load("E:\\timgallin\\projects\\kangaro\\config\\httoconf.test.xml") != KANGA_OK){
		LOG(ERROR) << "Load config failed.";
		return 0;
	}

	kangaro::PouchSvr tm;

	tm.Init();

	tm.RunSvr();

	return 0;	
}
