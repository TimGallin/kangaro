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

		InitBuffer();

		if (ProcessRequestLine(s, httpParam.http_request_uri) != KANGA_OK){
			return KANGA_ERROR;
		}

		if (ProcessHeaders(s, httpParam.http_headers) != KANGA_OK){
			return KANGA_ERROR;
		}

		if (httpParam.http_headers.find("Content-Length") != httpParam.http_headers.end()){
			if (ProcessBody(s, atol(httpParam.http_headers["Content-Length"].c_str()), httpParam.http_body) != KANGA_OK){
				return KANGA_ERROR;
			}
		}

		return KANGA_OK;
	}


	/*
	 * Description:
	 * 		Process request line,parse request-line.
	 */
	int HttpRecver::ProcessRequestLine(const socklen_t s, request_uri &http_request){
		int r = recv(s + 1, (char*)_request_buf->buffer, _request_buf->buffer_size, 0);
		_request_buf->valid_len = r;

		char* p = (char*)_request_buf->buffer;
		char c;
		for (_request_buf->last_read; _request_buf->last_read != r; ++_request_buf->last_read){
			c = *(p + _request_buf->last_read);
			if (c == LF){
				if (*(p + _request_buf->last_read - 1) == KCR){
					++_request_buf->last_read;
					break;
				}
			}
		}


		if (!RequestURIParse(std::string(_request_buf->buffer, _request_buf->buffer + _request_buf->last_read), http_request)){
			return KANGA_ERROR;
		}

		return KANGA_OK;
	}

	int HttpRecver::ProcessHeaders(const socklen_t s, kanga_headers& http_headers){
		std::string key = "", value = "";
		size_t line_start = _request_buf->last_read, line_end = 0;
		char* p = (char*)_request_buf->buffer;
		char c;
		for (_request_buf->last_read; _request_buf->last_read != _request_buf->valid_len; ++_request_buf->last_read){
			c = *(p + _request_buf->last_read);
			if (c == LF){
				if (*(p + _request_buf->last_read - 1) == KCR){
					line_end = _request_buf->last_read;

					if (line_start == line_end - 1){
						//CRLF CRLF means HTTP headers over
						++_request_buf->last_read;
						break;
					}

					//Parse header without CRLF
					if (HeaderParse(std::string(p + line_start, p + line_end - 1), key, value)){
						http_headers.insert(std::make_pair(std::move(key), std::move(value)));
					}
					
					line_start = line_end + 1;
					line_end = 0;
				}
			}
		}

		return KANGA_OK;
	}

	int HttpRecver::ProcessBody(const socklen_t s, const int len, kanga_body& http_body){
		size_t body_read = 0;
		http_body.reserve(len);
		while (_request_buf)
		{
			int part_start = _request_buf->last_read;
			for (_request_buf->last_read; _request_buf->last_read != _request_buf->valid_len; ++_request_buf->last_read){
				
				++body_read;
			}

			http_body.append((char*)(_request_buf->buffer + part_start), (char*)(_request_buf->buffer + _request_buf->last_read));

			if (body_read == len){
				break;
			}

			
			ExpandBuffer(_request_buf);
			_request_buf = _request_buf->next;
			
			int r = recv(s + 1, (char*)_request_buf->buffer, len - body_read, 0);
			
		}


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
		kangaro_request_buffer* gc = _head_buf;
		while (gc)
		{
			free(gc->buffer);
			_head_buf = _head_buf->next;
			delete gc;

			gc = _head_buf;
		} 
	}


}