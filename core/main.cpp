#include "NetworkController.h"
#include "Parser.h"
#include <iostream>


int main(int argc, char *argv[]){
	std::string datafile=argv[1];
	std::string controlfile=argv[2];
	std::string networkfile=argv[3];
	NetworkController c;
	if (argc == 5){
		std::string nodefile=argv[4];
		c.loadNetwork(nodefile);
	}
	c.loadNetwork(networkfile);
	c.loadObservations(datafile,controlfile);
	float time = 0.0;
	c.trainNetwork();
//	std::cout<<c.getNetwork()<<std::endl;
//	std::cout<<"Number of EM runs: "<<c.getNumberOfEMRuns()<<std::endl;
	for (int i = 1; i< 51; i++){
		time+=c.getTimeInMicroSeconds();
	}
	std::cout<<time/50<<std::endl;
	c.saveParameters();
	std::string input = "";
/*	std::cout<<"Please enter a query"<<std::endl;
	std::getline(std::cin,input);
	while (input != "exit"){
		try{
		Parser p3 = Parser(input,c);
		QueryExecuter qe3 = p3.parseQuery();
		std::pair<float, std::vector<std::string>> respair = qe3.execute();
		std::cout<<respair.first<<std::endl;
		for (const auto& arg: respair.second){
			std::cout<<arg<<"\n";
			}
		std::cout<<std::endl;
		}
		catch (std::exception& e){
			std::cerr<<e.what()<<std::endl;
		}
		std::cout<<"Please enter a query"<<std::endl;
		std::getline(std::cin,input);
	}*/
	return 0;
}
