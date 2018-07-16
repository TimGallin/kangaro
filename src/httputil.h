#ifndef KANGARO_HTTPUTIL_H
#define KANGARO_HTTPUTIL_H

#include "httpparams.h"

namespace kangaro{
	void kangaro_log(LOG_LEV level, const char* log);

	int kangaro_respond(HTTPMessage& _out);
}


#endif
