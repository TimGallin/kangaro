#include "http_parse.h"
#include "kangaro.h"
namespace kangaro{
	/*
	--------------------------------------
	HTTP URL Specification.
	http_URL = "http:" "//" host [ ":" port ] [ abs_path [ "?" query ]]
	--------------------------------------
	@param spec: The complete URL specification.
	*/
	bool URLParse(const std::string& spec, urlparts& patrs){
		patrs.spec = spec;

		enum{
			start = 0,
			schme_colon,
			schme_slash,
			host,
			port_colon_start,
			path_slash_start,
			query_question_mark_start
		}state = start;
		for each (const char& c in spec){

			switch (state)
			{
			case start:
				if (c == ':'){
					state = schme_colon;
					break;
				}
					
				patrs.scheme.push_back(c);
				break;

			case schme_colon:
				if (c == '/')
					state = schme_slash;
				break;

			case schme_slash:
				if (c == '/')
					state = host;
				break;
			case host:
				if (c == '/'){
					state = path_slash_start;
					break;
				}
					
				patrs.scheme.push_back(c);
				break;

			case port_colon_start:
				if (c == '/'){
					state = path_slash_start;
					break;
				}

				patrs.port.push_back(c);
				break;

			case path_slash_start:
				if (c == '?'){
					state = query_question_mark_start;
					break;
				}

				patrs.path.push_back(c);
				break;
			case query_question_mark_start:
				patrs.query.push_back(c);
				break;

			default:
				break;
			}
		}

		return true;
	}

	/*
	Parse HTTP Request-Uri
	Request-URI = "*" | absoluteURI | abs_path | authority
	*/
	bool RequestURIParse(const std::string& spec, request_uri& request){
		request.spec = spec;

		enum{
			option_start = 0,
			path_space_start,
			httpversion_space_start,
			version_slash_start
		}state = option_start;

		for each (const char& c in spec)
		{
			switch (state)
			{
			case option_start:
				if (c == ' '){
					state = path_space_start;
					break;
				}
				request.option.push_back(c);
				break;

			case path_space_start:
				if (c == ' '){
					state = httpversion_space_start;
					break;
				}

				request.abs_path.push_back(c);
				break;

			case httpversion_space_start:
				if (c == '/'){
					state = version_slash_start;
					break;
				}
				break;
			case version_slash_start:
				if (c == CR || c == LF){
					break;
				}
				request.version.push_back(c);
				break;

			default:
				break;
			}
		}

		return true;
	}

	/*
	Parse http header line.
	*/
	bool HeaderParse(const std::string& header_line, std::string& key, std::string& value){
		size_t colon = header_line.find(':');

		if (colon == std::string::npos){
			return false;
		}
		
		key = header_line.substr(0, colon);
		value = header_line.substr(colon + 1);

		return true;

	}
}