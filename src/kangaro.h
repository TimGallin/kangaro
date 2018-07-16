#ifndef KANGARO_MAIN_H
#define KANGARO_MAIN_H
<<<<<<< HEAD
#pragma once
=======

>>>>>>> 0254edb527ae9a09ea001894e1acb5057d587d00

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
