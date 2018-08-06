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
#define KANGARO_BACKLOG 10

#define Max_Client_HeaderSize 2048

#define KANGARO_SVRNAME_DLF "kangaro"

#define KANGARO_DLL_INTI_FUNCNAME "DllLoadin"
#define KANGARO_DLL_RELEASE_RES_FUNCNAME "DllReleaseRes"
#define KANGARO_DLL_UNLOAD_FUNCNAME "DllUnload"

#define LF     '\n'
#define KCR     '\r'
#define CRLF     "\r\n"
#define SP     ' '

#endif 
