#include "kangaro.h"
#include "httputil.h"
#include "httpsender.h"

namespace kangaro{
	void kangaro_log(LOG_LEV level, const char* log){
		if (log == NULL){
			return;
		}


		switch (level)
		{
		case LOG_LEV::KINFO:
			LOG(INFO) << log;
			break;

		case LOG_LEV::KWARNING:
			LOG(WARNING) << log;
			break;

		case LOG_LEV::KERROR:
			LOG(ERROR) << log;
			break;

		case LOG_LEV::KFATAL:
			LOG(FATAL) << log;
			break;
		default:
			break;
		}
	}

	int kangaro_respond(HTTPMessage& _out){

		return 0;
	}
}




