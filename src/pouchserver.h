#ifndef KANGARO_POUCHSERVER_H
#define  KANGARO_POUCHSERVER_H


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
	void Accept(int* pRfd);



	//Vars
	
	/*Listen Socket*/
	int _nSfd;
};



#endif  
