#include "kangaro.h"
#include "httprecver.h"

using namespace std;

/*
 * Description:
 * 		Recv on accepted socket. Parse the httpentity into HttpParams.
 */
bool HttpRecver::Process(int nSfd, HttpParams& httpParam){
	if(nSfd == -1){
		return false;
	}


	return true;
}


/*
 * Description:
 * 		Recv from socket.Split entity into rows.
 */
bool HttpRecver::Read(int nSfd, HttpParams& httpParam){
	char buf[1] = {0};
	int nMax = 1024;

	string sRow = "";
 	while(1){
		int r = recv(nSfd, buf, Max_Client_HeaderSize, 0);
		if(r !=1){
			return false;
		}

		if(buf[0] == '\r'){
			recv(nSfd, buf, 1, 0);
			if(buf[0] == '\n'){
				break;
			}
		}

		sRow += buf[0];
	}	
	
	ParseHeader(sRow, httpParam);
}


/*
 * Description:
 * 		Process request line,parse request-line.
 */
int HttpRecver::ProcessRequestLine(int nSfd, HttpParams& httpParam){
	return 0;
}


/*
 * Description:
 *		Split row of HttpHeader into k-v pair,insert into HttpParam.
 */
void HttpRecver::ParseHeader(const std::string& sRow, HttpParams& httpParam){
	size_t Colon = sRow.find(':');
	if(Colon == string::npos){
		return;
	}

	string sKey = sRow.substr(0, Colon-1);
	string sVal = sRow.substr(Colon+1, sRow.length());

	//Removing the key's leading and trailing whitespace.
	sKey.erase(0, sKey.find_first_not_of(" "));
	sKey.erase(sKey.find_last_not_of(" ") + 1);
	

	httpParam.SetHeader(sKey.c_str(), sVal.c_str());
}
