#ifndef DATADISTRIBUTION_H 
#define DATADISTRIBUTION_H

#include"Network.h"
#include"Combinations.h"
#include<map>

class DataDistribution{
	public:
	DataDistribution(Network& network, Matrix<int>& observations);
	private:
	void assignObservationsToNodes();
	int computeParentCombinations(std::vector<unsigned int> parents);
	void assignValueNames(Node& n);
	int getObservationColIndex(unsigned int sample, Node& n);
	int getObservationRowIndex(unsigned int sample, Node& n, Matrix<int>& obsMatrix);
	void assignParentNames(Node& n);
	void countObservations(Matrix<int>& obsMatrix, Node& n);
	void distributeObservations();
	Network& network_;	
	Matrix<int>& observations_;
	std::unordered_map<std::string,int>& observationsMap_;
	std::map<std::pair<int,int>, std::string>& observationsMapR_;
};

#endif
