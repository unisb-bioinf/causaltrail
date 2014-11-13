#ifndef PROBABILITYHANDLER_H
#define PROBABILITYHANDLER_H

#include "Network.h"
#include <boost/algorithm/string.hpp>

class ProbabilityHandler{
	public:
	ProbabilityHandler();
	ProbabilityHandler(Network& network);
	float computeTotalProbability(int nodeID, int value);
	float computeJointProbability(std::vector<unsigned int>& nodes, std::unordered_map<unsigned int,int>& values);
	float computeConditionalProbability(std::vector<unsigned int>& nodesNominator, std::vector<unsigned int>& nodesDenominator,
		 std::unordered_map<unsigned int, int>& valuesNominator, std::unordered_map<unsigned int, int>& valuesDenominator);
	std::pair<float,std::vector<std::string>> maxSearch(std::vector<unsigned int> queryNodes);
	private:
	std::vector<unsigned int> createFactorisation(std::vector<unsigned int>& queryNodes);
	std::unordered_map<unsigned int, std::vector<int>> assignValues(std::vector<unsigned int>& factorisation, std::unordered_map<unsigned int, int>& values);
	std::vector<std::vector<int>> enumerate(std::vector<unsigned int>& factorisation,std::unordered_map<unsigned int, std::vector<int>> valueAssignment);
	int getPosition(unsigned int& id, std::vector<unsigned int>& factorisation);
	int getParentValues(Node& n, std::vector<unsigned int>& factorisation, std::vector<int>& assignment);
	float computeFullySpecifiedProbabilityDistribution(std::vector<unsigned int>& factorisation, std::vector<std::vector<int>>& combinations);
	float computeFullySpecifiedProbabilityDistribution(std::vector<unsigned int>& factorisation, std::vector<int>& combinations);
	
	Network& network_;	
};

#endif
