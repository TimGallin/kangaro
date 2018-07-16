#ifndef KANGARO_MAIN_H
#define KANGARO_MAIN_H
#pragma once

#include "httpcode.h"

/*Platform*/

#ifdef _WIN32
#include "os\win.h"
#else

#endif


#include <glog\logging.h>

/*Implicit*/
#define KANGARO_BACKLOG 50

#define Max_Client_HeaderSize 1024

#define KANGARO_SVRNAME_DLF "kangaro"

#define LF     '\n'
#define KCR     '\r'
#define CRLF     "\r\n"
#define SP     ' '

#endif 
