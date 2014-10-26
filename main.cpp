//#include "Matrix.h"
#include "Controller.h"
#include <iostream>


int main(int argc, char *argv[]){
	Controller c= Controller();
	c.loadNetwork("TestA.na");
	c.loadNetwork("TestSif.sif");
	c.discretise("data.txt");
	c.distributeObservations();
	return 0;
}
