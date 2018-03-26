#include "kangaro.h"
#include "httprecver.h"

using namespace std;

/*
 * Description:
 * 		Recv on accepted socket. Parse the httpentity into HttpParams.
 */
bool HttpRecver::Process(int nSfd, HttpParams& httpParam){
	if(nSfd == -1){
		return false;
	}


	return true;
}


/*
 * Description:
 * 		Recv from socket.Split entity into rows.
 */
bool HttpRecver::Read(int nSfd, HttpParams& httpParam){

	Kanga_Http_RawRequest rawRequest;
	rawRequest.pRaw = new char[Max_Client_HeaderSize];
	rawRequest.nSize = Max_Client_HeaderSize;
	string sRow = "";
 	while(1){
		int r = recv(nSfd, buf, Max_Client_HeaderSize, 0);
		if(r !=1){
			return false;
		}

		if(buf[0] == '\r'){
			recv(nSfd, buf, 1, 0);
			if(buf[0] == '\n'){
				break;
			}
		}

		sRow += buf[0];
	}	
	
	ParseHeader(sRow, httpParam);
}


/*
 * Description:
 * 		Process request line,parse request-line.
 */
int HttpRecver::ProcessRequestLine(Kanga_Http_RawRequest& rawRequest, HttpParams& httpParam){

	
	for(;;){
		int r = recv(nSfd, rawRequest.pRaw, Max_Client_HeaderSize, 0);
		rawRequest.pTail += r;

		int n = ParseReqLine(rawRequest, httpParam);
		if(n == KANGA_OK){
			break;
		}
	}	
	return 0;
}


/*
 * Description:
 * 		Parse request line.
 */
int HttpRecver::ParseReqLine(Kanga_Http_RawRequest& rawRequest, HttpParams& httpParam){
	enum{
		sw_start = 0,
		sw_method,
		sw_sp_bf_uri,
		sw_uri,
		sw_sp_bf_http_schema,
		sw_http,
		sw_slash_bf_version,
		sw_version,
		sw_almost_done,
		sw_done
	}state;

	u_char* p = rawRequest.pPos;
	u_char* r = p;               //last reference

	state st = sw_start;
	for(p; p != rawRequest.pTail; ++p){
		u_char ch = *p;
		switch(st){
			case sw_start:
				if(ch == SP){
					st = sw_method;
					break;
				}
		     
				++p;
				break;

			case sw_method:
				if(ch == ' '){
					httpParam.GetHttpReqLine().sMethod.insert(0, r, (p - r));		
					st = sw_sp_bf_uri;
				}
				break;

			case sw_sp_bf_uri:
				r = p;
				st = sw_uri;
				break;

			case sw_uri:
				if(ch == ' '){
					httpParam.GetHttpReqLine().sUri.insert(0, r, (p - r));		
					st = sw_sp_bf_http_schema;
				}
				break;

			case sw_sp_bf_http_schema:
				r =p;
				st = sw_http;
				break

			case sw_http:
				if(ch == '/'){
					httpParam.GetHttpReqLine().sHttp,insert(0, r, (p-r));
					st = sw_slash_bf_version;
				}
				break

			case sw_slash_bf_version:
				r = p;
				st = sw_version;
				break

			case sw_version:
				if(ch == CR){
					httpParam.GetHttpReqLine().sHttp,insert(0, r, (p-r));
					st = sw_almost_done;
				}

				break

			case sw_almost_done:
				if(ch == LF){
					st = sw_done;
				}
				break

			case sw_done:
				r = p;
				rawRequest.pPos = r;
	 			return KANGA_OK;

			default:
				return -1;
		}
	}

	return KANGA_AGAIN;
}


/*
 * Description:
 *		Split row of HttpHeader into k-v pair,insert into HttpParam.
 */
void HttpRecver::ParseHeader(const std::string& sRow, HttpParams& httpParam){
	size_t Colon = sRow.find(':');
	if(Colon == string::npos){
		return;
	}

	string sKey = sRow.substr(0, Colon-1);
	string sVal = sRow.substr(Colon+1, sRow.length());

	//Removing the key's leading and trailing whitespace.
	sKey.erase(0, sKey.find_first_not_of(" "));
	sKey.erase(sKey.find_last_not_of(" ") + 1);
	

	httpParam.SetHeader(sKey.c_str(), sVal.c_str());
}
