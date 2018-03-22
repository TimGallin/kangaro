#ifndef KANGARO_HTTPPARAMS_H
#define KANGARO_HTTPPARAMS_H

#include <map>

class HttpParams{
public:
	/*
	 * Description:
	 * 		Get params by name.
	 */
	char* GetParameter(const char* pName);


	/*
	 * Description:
	 * 		Set parameter in pair of key-value
	 */		 
	template<typename T> 
	void SetParameter(const char* pKey, const T& tVal);

	/*
	 * Description:
	 * 		Remove k-v pair via key name.
	 */
	void RemoveParameter(const char* pKey);

private:
	std::map<std::string,std::string> _MapParams;
};

#endif
