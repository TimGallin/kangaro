#include "kangaro.h"
#include "httprecver.h"
#include "http_parse.h"
using namespace std;

namespace kangaro{
	HttpRecver::HttpRecver() :
		_head_buf(NULL),
		_request_buf(NULL){
	}

	HttpRecver::~HttpRecver(){
	}

	void HttpRecver::InitBuffer(){
		_head_buf = new kangaro_request_buffer;
		_head_buf->buffer = static_cast<unsigned char*>(malloc(Max_Client_HeaderSize));
		memset(_head_buf->buffer, 0, Max_Client_HeaderSize * sizeof(unsigned char));
		_head_buf->buffer_size = Max_Client_HeaderSize;
		_head_buf->last_read = 0;
		_head_buf->valid_len = 0;

		_head_buf->last = NULL;
		_head_buf->next = NULL;

		_request_buf = _head_buf;
	}

	/*
	 * Description:
	 * 		Recv on accepted socket. Parse the httpentity into HttpParams.
	 */
	int HttpRecver::Process(const socklen_t s, HTTPMessage& httpParam){
		if (s == kangaro_invalid_socket){
			return KANGA_ERROR;
		}

		ExpandBuffer(_request_buf);

		if (ProcessRequestLine(s, httpParam.http_url) != KANGA_OK){
			return KANGA_ERROR;
		}

		if (ProcessHeaders(s, httpParam.http_headers) != KANGA_OK){
			return KANGA_ERROR;
		}

		if (httpParam.http_headers.find("Content-Length") != httpParam.http_headers.end()){
			if (ProcessBody(s, httpParam.http_body) != KANGA_OK){
				return KANGA_ERROR;
			}
		}

		return KANGA_OK;
	}


	/*
	 * Description:
	 * 		Process request line,parse request-line.
	 */
	int HttpRecver::ProcessRequestLine(const socklen_t s, urlparts &http_url){
		int r = recv(s + 1, (char*)_request_buf->buffer, _request_buf->buffer_size, 0);
		_request_buf->valid_len = r;

		char* p = (char*)_request_buf->buffer;
		char c;
		for (_request_buf->last_read; _request_buf->last_read != r; ++_request_buf->last_read){
			c = *(p + _request_buf->last_read);
			if (c == LF){
				if (*(p + _request_buf->last_read - 1) == CR){
					break;
				}
			}
		}


		if (!URLParse(std::string(_request_buf->buffer, _request_buf->buffer + _request_buf->last_read), http_url)){
			return KANGA_ERROR;
		}

		return KANGA_OK;
	}

	int HttpRecver::ProcessHeaders(const socklen_t s, kanga_headers& http_headers){
		std::string key = "", value = "";
		size_t line_start = 0, line_end = 0;
		char* p = (char*)_request_buf->buffer;
		char c;
		for (_request_buf->last_read; _request_buf->last_read != _request_buf->valid_len; ++_request_buf->last_read){
			c = *(p + _request_buf->last_read);
			if (c == LF){
				if (*(p + _request_buf->last_read - 1) == CR){
					line_end = _request_buf->last_read;

					if (line_start == line_end - 1){
						//CRLF CRLF means HTTP headers over
						break;
					}

					if (HeaderParse(std::string(p + line_start, p + line_end), key, value)){
						http_headers.insert(std::make_pair(std::move(key), std::move(value)));
					}
					
					line_start = line_end + 1;
					line_end = 0;
				}
			}
		}

		return KANGA_OK;
	}

	int HttpRecver::ProcessBody(const socklen_t s, kanga_body& http_headers){



		return KANGA_OK;
	}

	void HttpRecver::ExpandBuffer(kangaro_request_buffer* buf){
		kangaro_request_buffer* next_buf = new kangaro_request_buffer;
		next_buf->buffer = static_cast<unsigned char*>(malloc(Max_Client_HeaderSize));
		memset(next_buf->buffer, 0, Max_Client_HeaderSize * sizeof(unsigned char));
		next_buf->buffer_size = Max_Client_HeaderSize;
		next_buf->last_read = 0;
		next_buf->valid_len = 0;

		next_buf->last = buf;
		next_buf->next = NULL;
	}

	void HttpRecver::FreeBuffer(kangaro_request_buffer* buf){

		if (buf->buffer){
			free(buf->buffer);
		}

		delete buf;
	}

	void HttpRecver::FreeAllBuffer(kangaro_request_buffer* head){

	}


}