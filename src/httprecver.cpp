#include "kangaro.h"
#include "httprecver.h"

using namespace std;


HttpRecver::HttpRecver(){
}

HttpRecver::~HttpRecver(){
}


/*
 * Description:
 * 		Recv on accepted socket. Parse the httpentity into HttpParams.
 */
int HttpRecver::Process(int nSfd, HttpParams& httpParam){
	if(nSfd == -1){
		return false;
	}

	Kanga_Http_RawRequest rawRequest;
	rawRequest.pRaw = new u_char[Max_Client_HeaderSize];
	rawRequest.nSize = Max_Client_HeaderSize;


	if(ProcessRequestLine(nSfd, rawRequest, httpParam) != KANGA_OK){
		return KANGA_ERROR;
	}
	
	if(ProcessHeaders(nSfd, rawRequest, httpParam) != KANGA_OK){
		return KANGA_ERROR;
	}

	return KANGA_OK;
}


/*
 * Description:
 * 		Process request line,parse request-line.
 */
int HttpRecver::ProcessRequestLine(int nSfd, Kanga_Http_RawRequest& rawRequest, HttpParams& httpParam){
	
	for(;;){
		int r = recv(nSfd, (char*)rawRequest.pPos, Max_Client_HeaderSize, 0);
		rawRequest.pTail += r;

		int n = ParseReqLine(rawRequest, httpParam);
		if(n == KANGA_OK){
			break;
		}

		if(n == KANGA_AGAIN){
			if(r == Max_Client_HeaderSize){
				ReallocParamReq(rawRequest);
			}

			continue;
		}

		return KANGA_ERROR;

	}	
	return KANGA_OK;
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
	}st = sw_start;

	u_char* p = rawRequest.pPos;
	u_char* r = p;               //last reference

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
					httpParam.GetHttpReqLine().sMethod.insert(0, (char*)r, (int)(p - r));		
					st = sw_sp_bf_uri;
				}
				break;

			case sw_sp_bf_uri:
				r = p;
				st = sw_uri;
				break;

			case sw_uri:
				if(ch == ' '){
					httpParam.GetHttpReqLine().sUri.insert(0, (char*)r, (int)(p - r));		
					st = sw_sp_bf_http_schema;
				}
				break;

			case sw_sp_bf_http_schema:
				r =p;
				st = sw_http;
				break;

			case sw_http:
				if(ch == '/'){
					httpParam.GetHttpReqLine().sHttp.insert(0, (char*)r, (int)(p-r));
					st = sw_slash_bf_version;
				}
				break;

			case sw_slash_bf_version:
				r = p;
				st = sw_version;
				break;

			case sw_version:
				if(ch == CR){
					httpParam.GetHttpReqLine().sVersion.insert(0, (char*)r, (int)(p-r));
					st = sw_almost_done;
				}

				break;

			case sw_almost_done:
				if(ch == LF){
					st = sw_done;
				}
				break;

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
int HttpRecver::ParseHeaders(Kanga_Http_RawRequest& rawRequest, HttpParams& httpParam){

	enum{
		sw_line_start=0,
		sw_line_almost_done,
		sw_headers_almost_done,
		sw_headers_done
	}st = sw_line_start;

	u_char* pStart = rawRequest.pPos;

	for(pStart; pStart != rawRequest.pTail; ++pStart){
		u_char ch = *rawRequest.pPos;

		switch(st){
			case sw_line_start:
				if(ch == CR){
					st = sw_line_almost_done;
				}
				break;

			case sw_line_almost_done:
				if(ch = LF){
					string sRow((char*)pStart,(int)(pStart - rawRequest.pPos));
					size_t Colon = sRow.find(':');
					if(Colon == string::npos){
						return KANGA_ERROR;
					}

					string sKey = sRow.substr(0, Colon-1);
					string sVal = sRow.substr(Colon+1, sRow.length());

					//Removing the key's leading and trailing whitespace.
					sKey.erase(0, sKey.find_first_not_of(" "));
					sKey.erase(sKey.find_last_not_of(" ") + 1);

					httpParam.SetHeader(sKey.c_str(), sVal.c_str());

					st = sw_headers_almost_done;
				}
				break;

			case sw_headers_almost_done:
				if(ch == CR){
					st = sw_headers_done;
				}else{
					st = sw_line_start;
				}
				break;
				
			case sw_headers_done:
				return KANGA_OK;
		}
				
	}

	return KANGA_AGAIN;

}


/*
 * Description: 
 * 		Realloc memory when buffer is too small.
 */
int HttpRecver::ReallocParamReq(Kanga_Http_RawRequest& rawRequest){
	int nNewSize = 2 * rawRequest.nSize;
	int nContent = rawRequest.pTail - rawRequest.pRaw;
	int nCurPos = rawRequest.pPos - rawRequest.pRaw;

	u_char* pNew = new u_char[nNewSize];
	memcpy(pNew, rawRequest.pRaw, nContent);

	delete[] rawRequest.pRaw;

	rawRequest.pRaw = pNew;
	rawRequest.pPos = pNew + nCurPos;
	rawRequest.nSize = nNewSize;

	pNew = NULL;

	return KANGA_OK;
}



/*
 * Description:
 * 		Process Headers.
 */
int HttpRecver::ProcessHeaders(int nSfd, Kanga_Http_RawRequest& rawRequest, HttpParams& httpParam){
	for(;;){
		int r = ParseHeaders(rawRequest, httpParam);

		if(r == KANGA_ERROR){
			return KANGA_ERROR;
		}

		if(r == KANGA_OK){
			break;
		}

		if(r == KANGA_AGAIN){
			ReallocParamReq(rawRequest);
			int r = recv(nSfd, (char*)rawRequest.pPos, Max_Client_HeaderSize, 0);
			rawRequest.pTail += r;
		}
	}

	return KANGA_OK;
}
