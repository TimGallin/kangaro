#include "kangaro.h"
#include "pouchserver.h"

#include <sys/socket.h>
#include <netdb.h>
#include <sys/un.h>


#ifdef WIN32
#define K_Getaddrinfo GetAddrInfo
#define K_FreeAddrInfo FreeAddrInfo

#else
#define K_Getaddrinfo getaddrinfo
#define K_FreeAddrInfo freeaddrinfo

#endif

using namespace std;

PouchSvr::PouchSvr(){
}

PouchSvr::~PouchSvr(){
}


/*
 * Description:
 *	Init.
 */
bool PouchSvr::Init(){
	/*Get addrinfo.*/
	struct addrinfo hints;
	struct addrinfo *result,*rp;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_TCP;

	int s = K_Getaddrinfo(NULL, KANGARO_PORT, &hints, &result);
	if(s != 0){
		cout<< "Getaddrinfo failed.\n";
		return false;		
	}

	/* getaddrinfo() returns a list of address structures.
           Try each address until we successfully bind(2).
           If socket(2) (or bind(2)) fails, we (close the socket
           and) try the next address. */

	for(rp = result; rp != NULL; rp->ai_next){
		/*Create socket.*/
		_nSfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if(_nSfd == -1){
			cout<< "Create socket failed.\n";
			continue;
		}

		/*Bind*/
		if(bind(_nSfd, rp->ai_addr, (int)rp->ai_addrlen) == 0){
			break; /*Success*/
		}
		
		cout<< "Bind failed.\n";

		close(_nSfd);
	}

	if( rp == NULL){
		/*No address succeeded.*/
		cout<< "Could not bind.\n";
		return false;
	}

	K_FreeAddrInfo(result);

	/*Listen*/
	if(listen(_nSfd, KANGARO_BACKLOG) != 0){
		cout<< "Listen failed.\n";
		close(_nSfd);
		return false;
	}

	return true;

}
