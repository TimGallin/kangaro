#ifndef KANGARO_HTTPPARAMS_H
#define KANGARO_HTTPPARAMS_H

#include <map>
#include <sstream>

typedef struct _tagReqLine{
	std::string sMethod;
	std::string sUri;
	std::string HttpVersion;
}Kanga_Http_Reqline;


typedef struct _tagHttpBody{
	std::string sBody;
	int nCachingFile:1;
	std::string sCachingFile;
}Kanga_Http_Body;

class HttpParams{
public:
	/*
	 * Description:
	 * 		Get http header by name.
	 */
	const char* GetHeader(const char* pName);


	/*
	 * Description:
	 * 		Set http header in pair of key-value
	 */		 
	void SetHeader(const char* pName, const char* pVal);

	/*
	 * Description:
	 * 		Remove http header by name.
	 */
	void RemoveHeader(const char* pKey);


	/*
	 * Description:
	 * 		Get http request line information.
	 */
	Kanga_Http_Reqline& GetHttpReqLine();


	/*
	 * Description:
	 * 		Get http body.
	 */
	Kanga_Http_Body& GetHttpBody();


	/*
	 * Description:
	 * 		Clear
	 */
	void Clear();

private:
	/*
	 * general-header & request-header & entity-header.
	 */
	std::map<std::string, std::string> _KangaHeaders;

	/*Request Line.*/
	Kanga_Http_Reqline _KangaReqLine;
	
	/*Http Body*/
	Kanga_Http_Body _KangaBody;
};

#endif
