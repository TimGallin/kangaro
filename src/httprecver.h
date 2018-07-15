#ifndef KANGARO_HTTPRECVER_H
#define KANGARO_HTTPRECVER_H
#pragma once
#include "httpparams.h"

namespace kangaro{
	class HttpRecver{
	public:
		HttpRecver();
		~HttpRecver();

		/*
		 * Description:
		 * 		Recv on accepted socket. Parse the httpentity into HttpParams.
		 */
		int Process(const socklen_t s, HTTPMessage& httpParam);

	private:

		/*
		 * Description:
		 * 		Process request line,parse request-line.
		 */
		int ProcessRequestLine(const socklen_t s, request_uri &http_request);

		/*
		 * Description:
		 * 		Process Headers.
		 */
		int ProcessHeaders(const socklen_t s, kanga_headers& http_headers);

		/*
		Description:
		 		Process Body.

		@param len : Content-Length
		*/
		int ProcessBody(const socklen_t s, const int len, kanga_body& http_body);

		void InitBuffer();

		void ExpandBuffer(kangaro_request_buffer* buf);

		void FreeBuffer(kangaro_request_buffer* buf);

		void FreeAllBuffer(kangaro_request_buffer* head);

		kangaro_request_buffer* _head_buf;
		kangaro_request_buffer* _request_buf;
	};

}

#endif
