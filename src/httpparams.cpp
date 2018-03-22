#include "kangaro.h"
#include "httpparams.h"

#include <sstream>

using namespace std;


/*
 * Description:
 * 		Get params by name.
 */
char* HttpParams::GetParameter(const char* pName){
	map<string,string>::iterator ite = _MapParams.find(key);

	if(ite == _MapPrams.end()){
		return NULL;
	}

	return &ite->second;
}


/*
 * Description:
 * 		Set parameter in pair of key-value
 */		 
template<typename T> 
void HttpParamsi<T>::SetParameter(const char* pKey, const T& tVal){
	stringstream ss;
	ss<<tval;
	
	map<string,string>::iterator ite = _MapParams.find(key);

	if(ite == _MapParams.end()){
		_MapParams.insert(pair<string,string>(pKey,ss.str()));
	}
	else{
		_MapParams[pKey] = ss.str();
	}
}


/*
 * Description:
 * 		Remove k-v pair via key name.
 */
void HttpParams::RemoveParameter(const char* pKey){
	map<string,string>::iterator ite = _MapParams.find(key);

	if(ite == _MapParams.end()){
		return;
	}

	_MapParams.erase(ite);
}
