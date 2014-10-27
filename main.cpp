#include "Controller.h"
#include <iostream>


int main(int argc, char *argv[]){
	Controller c= Controller();
	c.loadNetwork("TestA.na");
	c.loadNetwork("TestSif.sif");
	c.discretise("../../Prototyp/data_incomp10.txt","control.txt");
//	c.discretise("data2.txt");
	c.distributeObservations();
	return 0;
}
