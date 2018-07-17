#ifndef KANGARO_HTTPRECVER_H
#define KANGARO_HTTPRECVER_H
#pragma once
#include "httpparams.h"

namespace kangaro{
	class HttpRecver{
	public:
		/*
		--------------------------------------
		Request Buffer.
		--------------------------------------
		*/
		typedef struct _kangaro_request_buffer_{
			unsigned char* buffer;
			size_t buffer_size;
			size_t valid_len;
			size_t last_read;

			_kangaro_request_buffer_* last; //last buffer
			_kangaro_request_buffer_* next; //next buffer
		}kangaro_request_buffer;


		HttpRecver();
		~HttpRecver();

		/*
		 * Description:
		 * 		Recv on accepted socket. Parse the httpentity into HttpParams.
		 */
		int Process(const socklen_t s, HTTPMessage& httpParam);

	private:
		/*
		Recv
		*/
		int RecvAsyn(const socklen_t s, char* buffer, size_t size, int flags);

		/*
		 * Description:
		 * 		Process request line,parse request-line.
		 */
		int ProcessRequestLine(const socklen_t s, request_uri &http_request);

		/*
		 * Description:
		 * 		Process Headers.
		 */
		int ProcessHeaders(const socklen_t s, HTTPMessage& httpParam);

		/*
		Description:
		 		Process Body.

		@param len : Content-Length
		*/
		int ProcessBody(const socklen_t s, const int len, kanga_body& http_body);

		void ValidateChunked(const char* header);

		void InitBuffer();

		kangaro_request_buffer* ExpandBuffer(kangaro_request_buffer* buf, size_t space = Max_Client_HeaderSize);

		void FreeBuffer(kangaro_request_buffer* buf);

		void FreeAllBuffer(kangaro_request_buffer* head);

		kangaro_request_buffer* _head_buf;
		kangaro_request_buffer* _request_buf;

		bool _bChunked;
		size_t _content_length;
	};

}

#endif
