#ifndef KANGA_HTTP_CONFIG_HEAD
#define KANGA_HTTP_CONFIG_HEAD

class HttpConfig{
	HttpConfig();
	~HttpConfig();

	/*
	 * Description:
	 * 		Load config file,read it into memory.
	 */
	int Load(const char* pConfigFile);


};


#endif
