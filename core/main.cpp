#include "NetworkController.h"
#include "Parser.h"
#include <iostream>


int main(int argc, char *argv[]){
	NetworkController c;
	if (argc < 4) {
		std::cout
		    << "Insufficient number of parameters\n\n"
		    << "Usage:\n\t" << argv[0]
		    << " observations.txt discretisation_control.json network.tgf\n\n"
		    << "or:\n\t" << argv[0]
		    << " observations.txt discretisation_control.json network.sif "
		       "network.na\n";
	}
	else{
		std::string datafile=argv[1];
		std::string controlfile=argv[2];
		std::string networkfile=argv[3];
		if (argc == 5){
			std::string nodefile=argv[4];
			c.loadNetwork(nodefile);
		}
		c.loadNetwork(networkfile);
		c.loadObservations(datafile,controlfile);
		c.trainNetwork();
		std::cout<<c.getNetwork()<<std::endl;
		std::cout<<"Number of EM runs: "<<c.getNumberOfEMRuns()<<std::endl;
		std::cout<<"Time used for training: "<<c.getTimeInMicroSeconds()<<"µs"<<std::endl;
		std::string input = "";
		std::cout<<"Please enter a query"<<std::endl;
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
		}
	}
	return 0;
}
