#include "kangaro.h"
#include "httpsender.h"
#include "httpcode.h"
#include "httpconfig.h"

#include <map>

namespace kangaro {
	namespace httpsender{
		std::pair<std::string, std::string> pairArray[] =
		{
			make_pair(std::string(HTTP_STATUS_OK), "OK"),
			make_pair(std::string(HTTP_STATUS_NOT_IMPLEMENT), "Not Implement"),
			make_pair(std::string(HTTP_STATUS_NOT_FOUND), "Not Found"),
			make_pair(std::string(HTTP_STATUS_LENGTH_REQUIRED), "Length Required")

		
		};
		static std::map<std::string, std::string> http_status(pairArray, pairArray + sizeof(pairArray) / sizeof(pairArray[0]));

		int Respond(const socklen_t s, const char* status, HTTPRespond* httpRespond){
			if (http_status.find(status) == http_status.end()){
				LOG(ERROR) << "Invalid status code.";
				return KANGA_ERROR;
			}

			char buf[1024] = { 0 };

			//---------------------
			//Response Status-Line  HTTP/1.1 200 OK
			//---------------------
			sprintf(buf, "%s %s %s\r\n", HTTP_VERSION, status, http_status[status].c_str());
			send(s, buf, strlen(buf), 0);
			memset(buf, 0, sizeof(char) * 1024);

			//---------------------
			//Response general-header 
			//Cache - Control; Section 14.9
			//	| Connection; Section 14.10
			//	| Date; Section 14.18
			//	| Pragma; Section 14.32
			//	| Trailer; Section 14.40
			//	| Transfer - Encoding; Section 14.41
			//	| Upgrade; Section 14.42
			//	| Via; Section 14.45
			//	| Warning; Section 14.46
			//---------------------

			///*Connection*/
			//sprintf(buf, "Connection:keep-alive\r\n", HTTP_SERVER);
			//send(s, buf, strlen(buf), 0);
			//memset(buf, 0, sizeof(char) * 1024);

			//---------------------
			//Response response-header https://tools.ietf.org/html/rfc2616#section-6.2
			//Accept - Ranges; Section 14.5
			//	| Age; Section 14.6
			//	| ETag; Section 14.19
			//	| Location; Section 14.30
			//	| Proxy - Authenticate; Section 14.33
			//	| Retry - After; Section 14.37
			//	| Server; Section 14.38
			//	| Vary; Section 14.44
			//	| WWW - Authenticate; Section 14.47
			//---------------------

			/*Server*/
			sprintf(buf, "Server:%s\r\n", HTTP_SERVER);
			int n = strlen(buf);
			send(s, buf, strlen(buf), 0);
			memset(buf, 0, sizeof(char) * 1024);



			bool content_type_custom = false;

			if (httpRespond){
				while (httpRespond->http_headers){
					//If the Content-Type exist.
					if (strstr(httpRespond->http_headers->header, "Content-Type") != NULL){
						content_type_custom = true;
					}

					sprintf(buf, "%s%s", httpRespond->http_headers->header, CRLF);
					send(s, buf, strlen(buf), 0);
					memset(buf, 0, sizeof(char) * 1024);
				}

				//Content-Length
				sprintf(buf, "Content-Length:%d\r\n", httpRespond->http_body.len);
				send(s, buf, strlen(buf), 0);
				memset(buf, 0, sizeof(char) * 1024);
			}


			//---------------------
			//Response entity-header https://tools.ietf.org/html/rfc2616#section-7.1
			//Allow; Section 14.7
			//	| Content - Encoding; Section 14.11
			//	| Content - Language; Section 14.12
			//	| Content - Length; Section 14.13
			//	| Content - Location; Section 14.14
			//	| Content - MD5; Section 14.15
			//	| Content - Range; Section 14.16
			//	| Content - Type; Section 14.17
			//	| Expires; Section 14.21
			//	| Last - Modified; Section 14.29
			//	| extension - header
			//---------------------

			if (!content_type_custom){
				//Default Content-type
				sprintf(buf, "Content-Type:%s\r\n", HttpServerConfig::GetInstance()->GetDefaultContenttype());
				send(s, buf, strlen(buf), 0);
				memset(buf, 0, sizeof(char) * 1024);
			}


			/*extension - header*/
			if (HttpServerConfig::GetInstance()->IsCROSSupport()){
				strcpy(buf, "Access-Control-Allow-Origin:*\r\n");
				send(s, buf, strlen(buf), 0);
				memset(buf, 0, sizeof(char) * 1024);
			}

			//CRLF
			strcpy(buf, "\r\n");
			send(s, buf, strlen(buf), 0);
			memset(buf, 0, sizeof(char) * 1024);

			//Body
			if (httpRespond){
				send(s, httpRespond->http_body.httpbd, httpRespond->http_body.len, 0);
			}
			
			return KANGA_OK;
		}
	}


}

