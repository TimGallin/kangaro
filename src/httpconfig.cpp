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
		_http_port(""),
		_first_request(NULL),
		_back_log(0),
		_default_content_type(),
		_CROS_permission(false){
		config = this;

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
		

		_http_port = XML_ATTRIBUTE(http_element,"port");
		_back_log = atoi(XML_ATTRIBUTE(http_element, "backlog"));
		_default_content_type = XML_ATTRIBUTE(http_element, "dlf_content_type");
		_CROS_permission = strcmp(XML_ATTRIBUTE(http_element, "cros"), "true") ? true : false;

		//Load-in all Request-config
		tinyxml2::XMLElement* http_request = NULL;
		http_request = http_element->FirstChildElement("request");
		if (http_request == NULL){
			return KANGA_ERROR;
		}

		request_conf* _last_request = _first_request;
		while (http_request != NULL){
			_last_request = new request_conf;

			_last_request->type = 0;
			_last_request->path = XML_ATTRIBUTE(http_request, "path");
			_last_request->enter_point = XML_ATTRIBUTE(http_request, "enterpoint");
			_last_request->lib_path = XML_ATTRIBUTE(http_request, "libpath");

			if (_first_request == NULL){
				_first_request = _last_request;
			}

			_last_request = _last_request->next;

			http_request = http_request->NextSiblingElement("request");
		}

		return KANGA_OK;
	}

	const char* HttpServerConfig::GetHTTPPort(){
		return _http_port.c_str();
	}

	const char* HttpServerConfig::GetDefaultContenttype(){
		return _default_content_type.c_str();
	}

	bool HttpServerConfig::IsCROSSupport() const{
		return _CROS_permission;
	}

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

	void HttpServerConfig::LoadDefaultConfig(){
		if (_http_port.empty()){
			_http_port = "50203";
		}

		if (_default_content_type.empty()){
			_http_port = "application/x-www-form-urlencoded";
		}

		if (_back_log == 0){
			_back_log = 10;
		}

		
	}
}