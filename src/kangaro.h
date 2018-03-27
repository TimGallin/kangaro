#ifndef KANGARO_MAIN_H
#define KANGARO_MAIN_H


#include <stdio.h>
#include <iostream>
#include <string>
#include <errno.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netdb.h>
#include <sys/un.h>
#include <thread>
#include <pthread.h>


#include "httpcode.h"

/*Platform*/

#ifdef WIN32
#define K_Getaddrinfo GetAddrInfo
#define K_FreeAddrInfo FreeAddrInfo

#else
#define K_Getaddrinfo getaddrinfo
#define K_FreeAddrInfo freeaddrinfo

#endif


/*Implicit*/
#define KANGARO_PORT "50203"
#define KANGARO_BACKLOG 50

#define Max_Client_HeaderSize 1024


#define LF     (u_char) '\n'
#define CR     (u_char) '\r'
#define CRLF     "\r\n"
#define SP     ' '

#endif 
