#include "kangaro.h"

#include "pouchserver.h"

int main(int argc, char* argv[]){

	kangaro::PouchSvr tm;
	tm.Init();
	tm.RunSvr();
	return 0;	
}
