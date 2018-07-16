#ifndef KANGARO_HTTPPARAMS_H
#define KANGARO_HTTPPARAMS_H
#pragma once

#include <map>
#include <sstream>
namespace kangaro{

	/*
	--------------------------------------
	Request-URI.
	Request-URI = "*" | absoluteURI | abs_path | authority
	--------------------------------------
	*/
	typedef struct _request_uri_ {
		// HTTP Opiton.Reffer to https://tools.ietf.org/html/rfc2616#section-9.2
		char* option;

		//// Absolute path. Refer to https://tools.ietf.org/html/rfc2616#section-5.1.2.
		//char* ab_path;

		// Relative path. Refer to https://tools.ietf.org/html/rfc2616#section-5.1.2.
		char* abs_path;

		//HTTP VERSION
		char* version;
	} request_uri;

	/*
	--------------------------------------
	HTTP HEADER
	--------------------------------------
	*/
	typedef struct _kanga_headers_
	{
		char* header;
		_kanga_headers_* next;
	}kanga_headers;

	/*
	--------------------------------------
	HTTP BODY
	--------------------------------------
	*/
	typedef struct _kanga_body_{
		char* httpbd;
		size_t len; //Length of body.(Exclude '\0')
	}kanga_body;

	/*
	--------------------------------------
	HTTP Message.
	--------------------------------------
	*/
	typedef struct _HttpMsg_{
		request_uri http_request_uri;
		kanga_headers* http_headers;
		kanga_body http_body;
	}HTTPMessage;


	/*
	HTTP SERVER Handle.
	*/
	enum LOG_LEV{
		KINFO,
		KWARNING,
		KERROR,
		KFATAL
	};

	typedef void(handle_log)(LOG_LEV, const char*);

	typedef int(handle_respond)(HTTPMessage&);

	typedef struct _SvrHandle_{
		handle_log _handle_log;
		handle_respond _handle_respond;
	}SvrHandle;
}
#endif
