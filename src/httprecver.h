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
	 * 		Recv from socket.Split entity into rows.
	 */
	bool Read(int nSfd, HttpParams& httpParam);


	/*
	 * Description:
	 * 		Process request line,parse request-line.
	 */
	int ProcessRequestLine(int nSfd, HttpParams& httpParam);


	/*
	 * Description:
	 * 		Split row of HttpHeader into k-v pair,insert into HttpParam.
	 */
	void ParseHeader(const std::string& sRow, HttpParams& httpParam);
};



#endif
