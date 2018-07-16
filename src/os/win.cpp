#include "win.h"

namespace kangaro_os {
	int soc_init(){
		WSADATA wsaData;
		
		//initialze Winsock,The WSAStartup function is called to initiate use of WS2_32.dll.
		int iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iRet != 0){
			return WSAGetLastError();
		}

		return KANGA_OK;
	}

	int soc_nonblocking(kangaro_socket_t s){
		unsigned long  nb = 1;

		return ioctlsocket(s, FIONBIO, &nb);
	}

	int soc_blocking(kangaro_socket_t s){
		unsigned long  nb = 0;
		
		return ioctlsocket(s, FIONBIO, &nb);
	}

	void soc_close(kangaro_socket_t s){
		shutdown(s, SD_SEND);
		closesocket(s);
	}

	kangaro_module load_lib(const char* lib){
		return ::LoadLibraryA(lib);
	}

	void free_lib(kangaro_module h){
		FreeLibrary(h);
	}

	void* get_lib_funtion(kangaro_module h, const char* f){
		return ::GetProcAddress(h, f);
	}
}