#include "NetworkController.h"
#include "Interventions.h"
#include "Parser.h"
#include <iostream>


int main(int argc, char *argv[]){
	std::string datafile=argv[1];
	std::string controlfile=argv[2];
	NetworkController c= NetworkController();
	c.loadNetwork("TestA.na");
	c.loadNetwork("TestSif.sif");
	c.loadObservations(datafile,controlfile);
	c.trainNetwork();
	ProbabilityHandler prob = ProbabilityHandler(c.getNetwork());
	std::unordered_map<unsigned int,int> testValues;
	std::unordered_map<unsigned int,int> testValues2;
	testValues[2]=1;
	testValues2[2]=1;
	testValues[3]=0;
	testValues[5]=1;
	testValues[4]=1;
	std::vector<unsigned int> testNodes = {2};

	std::cout<<prob.computeJointProbability(testNodes,testValues)<<std::endl;

	testNodes = {4};
	std::cout<<prob.computeJointProbability(testNodes,testValues)<<std::endl;

	testNodes = {5};
	std::cout<<prob.computeJointProbability(testNodes,testValues)<<std::endl;

	testNodes = {3};
	std::cout<<prob.computeJointProbability(testNodes,testValues)<<std::endl;
	
	std::vector<unsigned int> nominator = {3,2};
	std::vector<unsigned int> denominator = {2};
	std::cout<<prob.computeConditionalProbability(nominator,denominator,testValues,testValues2)<<std::endl;

	std::cout<<prob.computeTotalProbability(2,0)<<std::endl;
	Interventions Int = Interventions(c);
	Int.createBackupOfNetworkStructure();
	Int.doIntervention("Difficulty","d1");
	Int.reverseDoIntervention("Difficulty","d1");
	std::cout<<prob.computeTotalProbability(2,0)<<std::endl;
	std::cout<<prob.computeTotalProbability(2,1)<<std::endl;
	std::cout<<prob.computeTotalProbability(2,2)<<std::endl;

	std::cout<<prob.maxSearch(denominator).first<<" "<<prob.maxSearch(denominator).second[0]<<std::endl;

	std::cout<<"test1"<<std::endl;	
	Parser p = Parser("? Grade = g1",c);
	QueryExecuter qe = p.parseQuery();
	std::cout<<qe<<std::endl;
	qe.execute();
	std::cout<<"test2"<<std::endl;
	Parser p1 = Parser("? Grade = g1 ! do Intelligence = i0 do Letter = l1",c);
	QueryExecuter qe1=p1.parseQuery();//.execute();
	std::cout<<qe1<<std::endl;
	qe1.execute();
	std::cout<<"test3"<<std::endl;
	Parser p2 = Parser("? argmax ( Grade )",c);
	QueryExecuter qe2=p2.parseQuery();//.execute();
	std::cout<<qe2<<std::endl;
	qe2.execute();
	std::string input = "";
	std::cout<<"Please enter a query"<<std::endl;
	std::getline(std::cin,input);
	while (input != "exit"){
		Parser p3 = Parser(input,c);
		QueryExecuter qe3 = p3.parseQuery();
		try {
		std::cout<<qe3<<std::endl;
		qe3.execute();
		}
		catch (std::exception& e){
			std::cerr<<e.what()<<std::endl;
		}
		std::cout<<"Please enter a query"<<std::endl;
		std::getline(std::cin,input);
	}
	return 0;
}
