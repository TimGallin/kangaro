#include "kangaro.h"
#include "httpparams.h"

#include <sstream>

using namespace std;


/*
 * Description:
 * 		Get params by name.
 */
const char* HttpParams::GetHeader(const char* pName){
	map<string,string>::iterator ite = _KangaHeaders.find(pName);

	if(ite == _KangaHeaders.end()){
		return NULL;
	}

	return ite->second.c_str();
}


/*
 * Description:
 * 		Set http header in pair of name-value
 */		 
//template<typename T> 
void HttpParams::SetHeader(const char* pName, const char* pVal){
	//std::stringstream ss;
	//ss<<tVal;

	map<string, string>::iterator ite = _KangaHeaders.find(pName);

	if(ite == _KangaHeaders.end()){
		_KangaHeaders.insert(pair<string, string>(pName, pVal));
	}
	else{
		_KangaHeaders[pName] = pVal;
	}
}


/*
 * Description:
 * 		Remove http header by name.
 */
void HttpParams::RemoveHeader(const char* pKey){
	map<string,string>::iterator ite = _KangaHeaders.find(pKey);

	if(ite == _KangaHeaders.end()){
		return;
	}

	_KangaHeaders.erase(ite);
}


/*
 * Description:
 * 		Get http request line information.
 */
Kanga_Http_Reqline& HttpParams::GetHttpReqLine(){
	return _KangaReqLine;
}


/*
 * Description:
 * 		Get http body.
 */
Kanga_Http_Body& HttpParams::GetHttpBody(){
	return _KangaBody;
}



/*
 * Description:
 * 		Clear
 */
void HttpParams::Clear(){
	_KangaHeaders.clear();

	map<string,string>().swap(_KangaHeaders);
}
