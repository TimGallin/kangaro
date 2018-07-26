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

			//Respond line  HTTP/1.1 200 OK
			sprintf(buf, "%s %s %s\r\n", HTTP_VERSION, status, http_status[status].c_str());
			send(s, buf, strlen(buf), 0);
			memset(buf, 0, sizeof(char) * 1024);

			//Server
			sprintf(buf, "server : %s\r\n", HTTP_SERVER);
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
				sprintf(buf, "Content-Length : %d\r\n", httpRespond->http_body.len);
				send(s, buf, strlen(buf), 0);
				memset(buf, 0, sizeof(char) * 1024);
			}


			if (!content_type_custom){
				//Default Content-type
				sprintf(buf, "Content-Type : %s\r\n", HttpServerConfig::GetInstance()->GetDefaultContenttype());
				send(s, buf, strlen(buf), 0);
				memset(buf, 0, sizeof(char) * 1024);
			}

			if (HttpServerConfig::GetInstance()->IsCROSSupport()){
				strcpy(buf, "Access-Control-Allow-Origin : *\r\n");
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

