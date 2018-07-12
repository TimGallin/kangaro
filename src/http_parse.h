#ifndef KANGARO_HTTP_PARSE_H
#define KANGARO_HTTP_PARSE_H

#pragma once

#include "httpparams.h"

namespace kangaro{
	/*
	--------------------------------------
	HTTP URL Specification.
	--------------------------------------
	@param spec: The complete URL specification.
	*/
	bool URLParse(const std::string& spec, urlparts& patrs);


	/*
	Parse http header line.
	*/
	bool HeaderParse(const std::string& header_line, std::string& key, std::string& value);
}

#endif