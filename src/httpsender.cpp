#include "kangaro.h"
#include "httpsender.h"
#include "httpcode.h"

#include <map>

namespace kangaro {
	namespace httpsender{
		std::pair<std::string, std::string> pairArray[] =
		{
			make_pair(std::string(HTTP_STATUS_NOT_IMPLEMENT), "Not Implement"),
		
		};
		static std::map<std::string, std::string> http_status(pairArray, pairArray + sizeof(pairArray) / sizeof(pairArray[0]));

		int Respond(const socklen_t s, const char* status, HTTPMessage & httpRespond){
			

			return 0;
		}
	}


}

