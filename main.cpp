#include "Controller.h"
#include <iostream>


int main(int argc, char *argv[]){
	Controller c= Controller();
	c.loadNetwork("TestA.na");
	c.loadNetwork("TestSif.sif");
//	c.discretise("data.txt","control.txt");
	c.discretise("../../Prototyp/data_incomp60.txt","control.txt");
	c.distributeObservations();
	c.performEM();
	return 0;
}
