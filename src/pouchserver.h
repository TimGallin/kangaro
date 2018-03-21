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

private:
	int _nSfd;
};



#endif  
