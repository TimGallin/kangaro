#include "http_parse.h"

namespace kangaro{
	/*
	--------------------------------------
	HTTP URL Specification.
	--------------------------------------
	@param spec: The complete URL specification.
	*/
	bool URLParse(const std::string& spec, urlparts& patrs){
		patrs.spec = spec;



		return true;
	}


	/*
	Parse http header line.
	*/
	bool HeaderParse(const std::string& header_line, std::string& key, std::string& value){
		size_t colon = header_line.find(':');

		if (colon == std::string::npos){
			return false;
		}
		
		key = header_line.substr(0, colon);
		value = header_line.substr(colon);

		return true;

	}
}