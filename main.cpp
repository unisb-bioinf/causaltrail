#include "NetworkController.h"
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
	std::unordered_map<unsigned int,std::string> testValues;
	std::unordered_map<unsigned int,std::string> testValues2;
	testValues[2]="g1";
	testValues2[2]="g1";
	testValues[3]="i0";
	testValues[5]="s1";
	testValues[4]="l1";
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

	return 0;
}
