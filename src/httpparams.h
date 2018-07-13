#ifndef KANGARO_HTTPPARAMS_H
#define KANGARO_HTTPPARAMS_H

#include <map>
#include <sstream>
namespace kangaro{

	/*
	--------------------------------------
	URL component parts.
	http_URL = "http:" "//" host [ ":" port ] [ abs_path [ "?" query ]]
	--------------------------------------
	*/
	typedef struct _urlparts {
		///
		// The complete URL specification.
		///
		std::string spec;

		///
		// Scheme component not including the colon (e.g., "http").
		///
		std::string scheme;

		///
		// Host component. This may be a hostname, an IPv4 address or an IPv6 literal
		// surrounded by square brackets (e.g., "[2001:db8::1]").
		///
		std::string host;

		///
		// Port number component.
		///
		std::string port;

		///
		// Path component including the first slash following the host.
		///
		std::string path;

		///
		// Query string component (i.e., everything following the '?').
		///
		std::string query;
	} urlparts;


	/*
	--------------------------------------
	Request-URI.
	Request-URI = "*" | absoluteURI | abs_path | authority
	--------------------------------------
	*/
	typedef struct _request_uri_ {
		// The complete Request-URI specification.
		std::string spec;

		// HTTP Opiton.Reffer to https://tools.ietf.org/html/rfc2616#section-9.2
		std::string option;

		// Absolute path. Refer to https://tools.ietf.org/html/rfc2616#section-5.1.2.
		std::string ab_path;

		// Relative path. Refer to https://tools.ietf.org/html/rfc2616#section-5.1.2.
		std::string abs_path;

		//HTTP VERSION
		std::string version;
	} request_uri;

	/*
	--------------------------------------
	HTTP HEADER
	--------------------------------------
	*/
	typedef std::map<std::string, std::string> kanga_headers;

	/*
	--------------------------------------
	HTTP BODY
	--------------------------------------
	*/
	typedef std::string kanga_body;

	/*
	--------------------------------------
	HTTP Message.
	--------------------------------------
	*/
	typedef struct _HttpMsg{
		urlparts http_url;
		request_uri http_request_uri;
		kanga_headers http_headers;
		kanga_body http_body;
	}HTTPMessage;

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
}
#endif
