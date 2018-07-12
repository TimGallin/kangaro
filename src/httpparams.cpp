#include "kangaro.h"
#include "httpparams.h"

#include <sstream>


namespace kangaro{
	/*
	--------------------------------------
	HTTP Request.
	--------------------------------------
	*/
	/*
	 * Description:
	 * 		Get params by name.
	 */
	const char* HttpRequest::GetHeader(const std::string& header){
		//kanga_headers::iterator ite = _headers.find(header);

		//if (ite == _headers.end()){
		//	return "";
		//}

		return _headers["header"].c_str();
	}


	/*
	 * Description:
	 * 		Set http header in pair of name-value
	 */
	void HttpRequest::SetHeader(const std::string& header, const std::string& pVal){
		kanga_headers::iterator ite = _headers.find(header);

		if (ite == _headers.end()){
			_headers.insert(std::pair<std::string, std::string>(std::move(header), std::move(pVal)));
		}
		else{
			_headers[header] = pVal;
		}
	}


	/*
	 * Description:
	 * 		Get http body.
	 */
	const char* HttpRequest::GetHttpBody(){
		return _sBody.c_str();
	}



	/*
	--------------------------------------
	HTTP Respond.
	--------------------------------------
	*/
	HttpRespond::HttpRespond()
	{
	}

	HttpRespond::~HttpRespond()
	{
	}
}