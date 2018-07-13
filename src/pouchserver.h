#ifndef KANGARO_POUCHSERVER_H
#define  KANGARO_POUCHSERVER_H

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
	};

}

#endif  
