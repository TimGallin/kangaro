#ifndef KANGARO_OS_WIN
#define KANGARO_OS_WIN

#pragma once
#include <kangaro.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
typedef SOCKET  kangaro_socket_t;
typedef int     socklen_t;


//error
#define kangaro_invalid_socket INVALID_SOCKET 
#define kangaro_errno                  GetLastError()
#define kangaro_socket_errno           WSAGetLastError()

#define kangaro_ewouldblock WSAEWOULDBLOCK
#define kangaro_socket_close closesocket
#define kangaro_socket_shutdown shutdown
namespace kangaro_soc{
	int soc_init();

	int soc_nonblocking(kangaro_socket_t s);

	int soc_blocking(kangaro_socket_t s);
}


#endif