#include "kangaro.h"
#include "httpconfig.h"

//TinyXml2
#include "tinyxml2/tinyxml2.h"

#define XML_ATTRIBUTE(element,name)  (element->Attribute(name) == NULL) ? "":element->Attribute(name)

namespace kangaro{
	using namespace std;

	namespace{
		HttpServerConfig* config = NULL;
	}

	HttpServerConfig::HttpServerConfig():
		_http_port("50203"),
		_first_request(NULL),
		_back_log(10){
		config = this;

		_first_request = new request_conf;
		memset(_first_request, 0, sizeof(request_conf));
	}

	HttpServerConfig::~HttpServerConfig(){
		FreeAllConfig();
	}

	HttpServerConfig* HttpServerConfig::GetInstance(){
		return config;
	}

	int HttpServerConfig::Load(const char* pConfigFile){
		tinyxml2::XMLDocument doc;

		if (doc.LoadFile(pConfigFile) != tinyxml2::XMLError::XML_SUCCESS){
			return KANGA_ERROR;
		}

		tinyxml2::XMLElement* http_element = NULL;
		http_element = doc.FirstChildElement("config")->FirstChildElement("http")->ToElement();
		
		//Select port to listen.
		const char* port = http_element->Attribute("port");
		if (port){
			_http_port = port;
		}

		//Get Back-Log
		const char* blog = http_element->Attribute("backlog");
		if (blog){
			_back_log = atoi(port);
		}

		//Load-in all Request-config
		tinyxml2::XMLElement* http_request = NULL;
		http_request = http_element->FirstChildElement("request");
		if (http_request == NULL){
			return KANGA_ERROR;
		}

		request_conf* _last_request = _first_request;
		while (http_request != NULL){
			request_conf* link_request = new request_conf;

			link_request->type = 0;
			link_request->path = XML_ATTRIBUTE(http_request, "path");
			link_request->enter_point = XML_ATTRIBUTE(http_request, "enterpoint");
			link_request->lib_path = XML_ATTRIBUTE(http_request, "libpath");

			_last_request->next = link_request;
			_last_request = link_request;

			http_request = http_request->NextSiblingElement("request");
		}

		return KANGA_OK;
	}

	std::string HttpServerConfig::GetHTTPPort() const{
		return _http_port;
	}

	/*
	Description:
	Select a request-config via path.
	*/
	request_conf* HttpServerConfig::SelectRequestConfig(const std::string& name){
		request_conf* node = _first_request;

		while (node != NULL){
			if (name == node->path){
				return node;
			}

			node = node->next;
		}

		return NULL;
	}

	void HttpServerConfig::FreeAllConfig(){
		request_conf* prinsoner = NULL;
		while (_first_request != NULL){
			prinsoner = _first_request->next;
			delete _first_request;
			_first_request = prinsoner;
		}
		prinsoner = NULL;
		_first_request = NULL;
	}
}