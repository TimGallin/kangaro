#ifndef KANGARO_HTTPPARAMS_H
#define KANGARO_HTTPPARAMS_H

#include <map>
#include <sstream>
namespace kangaro{

	/*
	--------------------------------------
	URL component parts.
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
		// User name component.
		///
		std::string username;

		///
		// Password component.
		///
		std::string password;

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
		// Origin contains just the scheme, host, and port from a URL. Equivalent to
		// clearing any username and password, replacing the path with a slash, and
		// clearing everything after that. This value will be empty for non-standard
		// URLs.
		///
		std::string origin;

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

		kangaro_request_buffer* last; //last buffer
		kangaro_request_buffer* next; //next buffer
	}kangaro_request_buffer;
}
#endif
