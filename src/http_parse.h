#ifndef KANGARO_HTTP_PARSE_H
#define KANGARO_HTTP_PARSE_H

#pragma once

#include "httpparams.h"

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
	HTTP URL Specification.
	--------------------------------------
	@param spec: The complete URL specification.
	*/
	bool URLParse(const std::string& spec, urlparts& patrs);

	/*
	Parse HTTP Request-Uri
	Request-URI = "*" | absoluteURI | abs_path | authority
	*/
	bool RequestURIParse(char* spec_start, char* spec_end, request_uri& request);

	/*
	Parse http header line.
	*/
	bool HeaderParse(const std::string& header_line, std::string& key, std::string& value);
}

#endif