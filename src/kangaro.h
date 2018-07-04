#ifndef KANGARO_MAIN_H
#define KANGARO_MAIN_H


#include <stdio.h>
#include <iostream>
#include <string>
#include <errno.h>



#include <thread>


//TinyXml2
#include "tinyxml2/tinyxml2.h"


#include "httpcode.h"

/*Platform*/

#ifdef _WIN32
#include "os\win.h"
#else
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/un.h>


#define K_Getaddrinfo getaddrinfo
#define K_FreeAddrInfo freeaddrinfo

#endif

#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog\logging.h>

/*Implicit*/
#define KANGARO_PORT "50203"
#define KANGARO_BACKLOG 50

#define Max_Client_HeaderSize 1024


#define LF     (u_char) '\n'
#define CR     (u_char) '\r'
#define CRLF     "\r\n"
#define SP     ' '

#endif 
