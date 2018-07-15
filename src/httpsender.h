#ifndef KANGARO_HTTPSENDER_H
#define KANGARO_HTTPSENDER_H

#pragma once
#include "httpparams.h"

namespace kangaro {
	class HttpSender {
	public:
		HttpSender();
		~HttpSender();

		/*
		* Description:
		* 		Send Respond
		*/
		int Process(const socklen_t s, HTTPMessage& httpRespond);

	private:


	};

}


#endif
