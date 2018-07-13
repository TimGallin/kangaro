#include "kangaro.h"
#include "pouchserver.h"
#include "httprecver.h"
#include "httpconfig.h"
namespace kangaro{
	PouchSvr::PouchSvr(){
	}

	PouchSvr::~PouchSvr(){
	}


	/*
	 * Description:
	 *	Init.
	 */
	bool PouchSvr::Init(){
		kangaro_soc::soc_init();

		struct addrinfo hints;
		struct addrinfo *result, *rp;

		memset(&hints, 0, sizeof(struct addrinfo));

		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		hints.ai_protocol = IPPROTO_TCP;

		int s = getaddrinfo(NULL, HttpServerConfig::GetInstance()->GetHTTPPort().c_str(), &hints, &result);
		if (s != 0){
			LOG(ERROR) << "getaddrinfo failed.wsa error:" << kangaro_socket_errno;
			return false;
		}

		/* getaddrinfo() returns a list of address structures.
			   Try each address until we successfully bind(2).
			   If socket(2) (or bind(2)) fails, we (close the socket
			   and) try the next address. */

		for (rp = result; rp != NULL; rp->ai_next){
			/*Create socket.*/
			_listen_sd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
			if (_listen_sd == kangaro_invalid_socket){
				LOG(ERROR) << "Create socket failed.wsa error:" << kangaro_socket_errno;
				continue;
			}

			/*Bind*/
			if (bind(_listen_sd, rp->ai_addr, (int)rp->ai_addrlen) == 0){
				break; /*Success*/
			}

			LOG(ERROR) << "Bind failed.wsa error:" << kangaro_socket_errno;

			kangaro_socket_close(_listen_sd);
		}

		if (rp == NULL){
			/*No address succeeded.*/
			LOG(ERROR) << "Could not bind.wsa error:" << kangaro_socket_errno;
			return false;
		}

		freeaddrinfo(result);

		/*Listen*/
		if (listen(_listen_sd, KANGARO_BACKLOG) != 0){
			LOG(ERROR) << "Listen failed.wsa error:" << kangaro_socket_errno;
			kangaro_socket_close(_listen_sd);
			return false;
		}

		kangaro_soc::soc_nonblocking(_listen_sd);

		return true;

	}


	/*
	 * Description:
	 * 	Run
	 */
	void PouchSvr::RunSvr(){
		int i, len, rc, on = 1;
		int    desc_ready;
		BOOL end_server = FALSE;

		kangaro_socket_t incoming = 1;
		sockaddr_in user_peer;
		/*Initialize the master fd_set  */
		fd_set master_set, working_set;
		FD_ZERO(&master_set);
		FD_SET(_listen_sd, &master_set);

		/*Initialize the timeval struct to 1 minute.*/
		timeval tm = { 60, 0 };

		while (true){
			/* Copy the master fd_set over to the working fd_set.*/
			memcpy(&working_set, &master_set, sizeof(master_set));

			rc = select(_listen_sd + 1, &working_set, NULL, NULL, &tm);

			if (rc < 0){
				LOG(ERROR) << "select() failed." << kangaro_socket_errno;
				end_server = 1;
				break;
			}

			if (rc == 0){
				LOG(WARNING) << "select() timeout";
				continue;
			}

			desc_ready = rc;
			for (i = 0; i <= (int)master_set.fd_count && desc_ready > 0; ++i){
				/* Check to see if this descriptor is ready            */
				if (FD_ISSET(master_set.fd_array[i], &working_set)){
					/* A descriptor was found that was readable*/
					desc_ready -= 1;

					/* Check to see if this is the listening socket     */
					if (master_set.fd_array[i] == _listen_sd){
						LOG(INFO) << "Listening socket is readable.";

						/**********************************************/
						/* Accept each incoming connection.  If       */
						/* accept fails with EWOULDBLOCK, then we     */
						/* have accepted all of them.  Any other      */
						/* failure on accept will cause us to end the */
						/* server.                                    */
						/**********************************************/
						while (incoming){
							memset(&user_peer, 0, sizeof(user_peer));
							len = sizeof(user_peer);
							incoming = accept(_listen_sd, (PSOCKADDR)&user_peer, &len);
							if (incoming == kangaro_invalid_socket){
								if (kangaro_socket_errno != kangaro_ewouldblock){
									LOG(ERROR) << "accept() failed. " << kangaro_socket_errno;
									end_server = TRUE;
									break;
								}

								break;
							}

							FD_SET(incoming, &master_set);
						}

					}
					else{
						/**********************************************/
						/* Receive data on this connection until the  */
						/* recv fails with EWOULDBLOCK.  If any other */
						/* failure occurs, we will close the          */
						/* connection.                                */
						/**********************************************/
						Accept(master_set.fd_array[i]);
					}
				}

				if (end_server){
					break;
				}
			}

		}


	}


	/*
	 *Description:
	 *	Accept
	 * Params:
	 * 	int* pRfd : accepted socket
	 */
	void PouchSvr::Accept(int s){
		if (s == kangaro_invalid_socket){
			return;
		}

		HTTPMessage httpmsg;

		HttpRecver recver;
		if (recver.Process(s, httpmsg) != KANGA_OK){
			return;
		}

	}

}



