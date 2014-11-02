#include "Controller.h"
#include <iostream>


int main(int argc, char *argv[]){
	std::string datafile=argv[1];
	std::string controlfile=argv[2];
	Controller c= Controller();
	c.loadNetwork("TestA.na");
	c.loadNetwork("TestSif.sif");
	std::cout<<datafile<<" "<<controlfile<<std::endl;
	c.discretise(datafile,controlfile);
	c.trainNetwork();
	return 0;
}
