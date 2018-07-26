#ifndef KANGARO_POUCHSERVER_H
#define  KANGARO_POUCHSERVER_H
#include "httpdispatcher.h"
#include "httpparams.h"
namespace kangaro{
	class PouchSvr{
	public:
		PouchSvr();
		~PouchSvr();

		/*
		 * Description:
		 *	Init.
		 */
		bool Init();

		/*
		 * Description:
		 * 	Run
		 */
		void RunSvr();

	private:
		/*
		Go through all the <Init> config. Call DllInitWhenLoadin() for every dll;
		*/
		void DllsInit();

		/*
		 *Description:
		 *	Accept
		 *
		 * Params:
		 * 	int* pRfd : accepted socket
		 */
		void Accept(int s);



		//Vars

		/*Listen Socket*/
		kangaro_socket_t _listen_sd;

		HttpDispatcher _dispatcher;
	};

}

#endif  
