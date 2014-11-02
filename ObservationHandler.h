#ifndef OBSERVATIONHANDLER_H
#define OBSERVATIONHANDLER_H

#include"Network.h"
#include"Combinations.h"
#include<map>

class ObservationHandler{
	public:
	ObservationHandler(Network& network, Matrix<int>& observations, std::unordered_map<std::string,int>& observationsMap, std::map<std::pair<int,int>, std::string>& observationsMapR);
	private:
	void assignObservationsToNodes();
	int computeParentCombinations(std::vector<unsigned int> parents);
	void assignValueNames(Node& n);
	void assignParentNames(Node& n);
	void countObservations(Matrix<int>& obsMatrix, Node& n);
	void distributeObservations();
	Network& network_;	
	Matrix<int>& observations_;
	std::unordered_map<std::string,int>& observationsMap_;
	std::map<std::pair<int,int>, std::string>& observationsMapR_;
};

#endif
