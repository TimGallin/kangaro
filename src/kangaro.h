#ifndef KANGARO_MAIN_H
#define KANGARO_MAIN_H


#include <stdio.h>
#include <iostream>
#include <string>
#include <errno.h>
#include <thread>




#include "httpcode.h"

/*Platform*/

#ifdef _WIN32
#include "os\win.h"
#else

#endif

#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog\logging.h>

/*Implicit*/
#define KANGARO_BACKLOG 50

#define Max_Client_HeaderSize 1024


#define LF     '\n'
#define CR     '\r'
#define CRLF     "\r\n"
#define SP     ' '

#endif 
