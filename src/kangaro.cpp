#include "kangaro.h"

#include "pouchserver.h"

int main(int argc, char* argv[]){
	PouchSvr tm;
	tm.Init();
	tm.RunSvr();
	return 0;	
}
