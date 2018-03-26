#ifndef KANGARO_HTTPRECVER_H
#define KANGARO_HTTPRECVER_H

#include "httpparams.h"

class HttpRecver{
public:
	HttpRecver();
	~HttpRecver();
	
	/*
	 * Description:
	 * 		Recv on accepted socket. Parse the httpentity into HttpParams.
	 */
	bool Process(int nSfd, HttpParams& httpParam);
	
private:
	
	/*
	 * Description: 
	 * 		Realloc memory when buffer is too small.
	 */
	int ReallocParamReq(u_char* pStart, size_t sLen);
		

	/*
	 * Description:
	 * 		Recv from socket.Split entity into rows.
	 */
	bool Read(int nSfd, HttpParams& httpParam);


	/*
	 * Description:
	 * 		Process request line,parse request-line.
	 */
	int ProcessRequestLine(Kanga_Http_RawRequest& rawRequest, HttpParams& httpParam);


	/*
	 * Description:
	 * 		Parse request line.
	 */
	int ParseReqLine(Kanga_Http_RawRequest& rawRequest, HttpParams& httpParam);


	/*
	 * Description:
	 * 		Split row of HttpHeader into k-v pair,insert into HttpParam.
	 */
	void ParseHeader(const std::string& sRow, HttpParams& httpParam);
};



#endif
