#include "kangaro.h"
#include "httprecver.h"
#include "http_parse.h"
using namespace std;

namespace kangaro{
	HttpRecver::HttpRecver() :
		_head_buf(NULL),
		_request_buf(NULL),
		_bChunked(true),
		_content_length(0){
	}

	HttpRecver::~HttpRecver(){
		FreeAllBuffer(_head_buf);
	}

	void HttpRecver::InitBuffer(){
		_head_buf = new kangaro_request_buffer;
		memset(_head_buf, 0, sizeof(kangaro_request_buffer));

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

		if (ProcessHeaders(s, httpParam) != KANGA_OK){
			return KANGA_ERROR;
		}

		if (!_bChunked){
			if (ProcessBody(s, _content_length, httpParam.http_body) != KANGA_OK){
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

		int r = RecvAsyn(s, (char*)_request_buf->buffer, _request_buf->buffer_size, 0);
		if (r < 0){
			return KANGA_ERROR;
		}

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


		if (!RequestURIParse((char*)_request_buf->buffer, (char*)_request_buf->buffer + _request_buf->last_read, http_request)){
			LOG(ERROR) << "parse request-uri failed.";
			return KANGA_ERROR;
		}
		//Log request uri.
		LOG(INFO) << _request_buf->buffer;

		return KANGA_OK;
	}

	int HttpRecver::ProcessHeaders(const socklen_t s, HTTPMessage& httpParam){
		kanga_headers* last = NULL;
		httpParam.http_headers = NULL;
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
						//CRLF CRLF means HTTP headers end
						++_request_buf->last_read;
						break;
					}

					//Parse header without CRLF
					if (last == NULL){
						httpParam.http_headers = new kanga_headers;
						memset(httpParam.http_headers, 0, sizeof(kanga_headers));
						last = httpParam.http_headers;
					}
					

					last->header = p + line_start;
					*(p + line_end - 1) = 0;
					ValidateChunked(last->header);

					LOG(INFO) << last->header;

					//Reverse-Order.
					kanga_headers* next_header = new kanga_headers;
					memset(next_header, 0, sizeof(kanga_headers));

					last->next = next_header;
					last = next_header;
			
					line_start = line_end + 1;
					line_end = 0;
				}
			}
		}

		return KANGA_OK;
	}

	int HttpRecver::ProcessBody(const socklen_t s, const int len, kanga_body& http_body){
		size_t body_read = 0;
		http_body.len = len;
		http_body.httpbd = (char*)_request_buf->buffer + _request_buf->last_read;

		while (_request_buf)
		{
			//int part_start = _request_buf->last_read;
			body_read += _request_buf->valid_len - _request_buf->last_read;
			_request_buf->last_read = _request_buf->valid_len;

			if (body_read == len){
				//The buffer contains all the content.
				break;
			}

			size_t remainder_space = _request_buf->buffer_size - _request_buf->valid_len;//Remainder space of request buffer.

			if (remainder_space < len - body_read){
				/*
				The space required for content is far greater than this buffer.
				Create a new buffer in body size.Move the valid part of body to new buffer.Set body pointer to the start of new one.
				*/
				
				kangaro_request_buffer* body_buffer = ExpandBuffer(_request_buf, len + 1);
				memmove(body_buffer->buffer, http_body.httpbd, body_read);
				memset(http_body.httpbd, 0, body_read*sizeof(unsigned char));

				body_buffer->valid_len = body_read;
				http_body.httpbd = (char*)body_buffer->buffer;
				body_buffer->last_read = body_read;
				_request_buf = body_buffer;

			}
			else
			{
				//Remainder space is enough.Just read the remaining part of content body.
				int r = RecvAsyn(s, (char*)_request_buf->buffer + _request_buf->last_read, remainder_space, 0);
				if (r < 0){
					return KANGA_ERROR;
				}

				body_read += r;
			}
	
			if (body_read == len){
				//The buffer contains all the content.
				break;
			}
			
		}

		LOG(INFO)<<"HTTP Body:\r\n" << http_body.httpbd;

		return KANGA_OK;
	}

	HttpRecver::kangaro_request_buffer* HttpRecver::ExpandBuffer(kangaro_request_buffer* buf, size_t space){
		kangaro_request_buffer* next_buf = new kangaro_request_buffer;
		next_buf->buffer = static_cast<unsigned char*>(malloc(space));
		memset(next_buf->buffer, 0, space * sizeof(unsigned char));
		next_buf->buffer_size = space;
		next_buf->last_read = 0;
		next_buf->valid_len = 0;

		next_buf->last = buf;
		next_buf->next = NULL;

		buf->next = next_buf;

		return next_buf;
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

	void HttpRecver::ValidateChunked(const char* header){
		if (strstr(header, "Content-Length") != NULL){
			_bChunked = false;

			const char* colon = strchr(header, ':');
			_content_length = atoi(colon + 1);
		}
	}

	int HttpRecver::RecvAsyn(const socklen_t s, char* buffer, size_t size, int flags){
		int r = 0;
		while (1)
		{
			r = recv(s, buffer, size, flags);
			if (r < 0){
				if (kangaro_socket_errno != kangaro_ewouldblock){
					LOG(ERROR) << "recv() error." << kangaro_socket_errno;
					return -1;
				}
				continue;
			}
			else
			{
				break;
			}
		}
		
		return r;
	}
}