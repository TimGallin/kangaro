#ifndef KANGARO_HTTPSENDER_H
#define KANGARO_HTTPSENDER_H

#pragma once
#include "httpparams.h"

namespace kangaro {
	namespace httpsender{
		/*
		* Description:
		* 		Send Respond
		*/
		int Respond(const socklen_t s, const char* status, HTTPMessage& httpRespond);
	}

}


#endif
