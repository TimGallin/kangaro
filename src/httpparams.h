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
	HTTP Message. (Request)
	--------------------------------------
	*/
	typedef struct _HttpMsg_{
		request_uri http_request_uri;
		kanga_headers* http_headers;
		kanga_body http_body;
	}HTTPMessage;

	/*
	--------------------------------------
	HTTP Message. (Respond)
	--------------------------------------
	*/
	typedef struct _HttpRespond_{
		kanga_headers* http_headers;
		kanga_body http_body;
		int need_release; // 0: false 1:true
	}HTTPRespond;


	/*
	Server£º
		DLL Funtion
		int (HTTPMessage* _in,HTTPRespond* _out){
			//Log
			_handle->_handle_log(LOG_LEV::KINFO,"Hello World");

			//Make Respond.(_out)

			//Add headers. (Do not add /r/n)

			return 0;
			}
		
		
	
		...
	*/

	//---------------------------------------------
	//Dynamic lib Function Type.
	//---------------------------------------------

	//Load Init 
	//Name :"DllLoadin"
	typedef int(*http_dlib_loadinit)();

	//Respond release 
	//Name :"DllReleaseRes"
	typedef int(*http_dlib_release)(HTTPRespond* respond);

	//Unload
	//Name :"DllUnload"
	//Remark: This function will be called before freelibrary().
	typedef int(*http_dlib_unload)();

	//Handle
	typedef int(*http_dlib_enter_point) (HTTPMessage* _in, HTTPRespond* _out);

}
#endif
