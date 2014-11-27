#ifndef PROBABILITYHANDLER_H
#define PROBABILITYHANDLER_H

#include "Network.h"
#include <boost/algorithm/string.hpp>

class ProbabilityHandler{
	public:
	ProbabilityHandler();
	ProbabilityHandler(Network& network);
	float computeTotalProbability(int nodeID, int value);
	float computeTotalProbabilityNormalized(int nodeID, int value);
	float computeJointProbability(const std::vector<unsigned int>& nodes, std::unordered_map<unsigned int, int>& values);
	float computeJointProbabilityWithoutNormalization(const std::vector<unsigned int>& nodes, std::unordered_map<unsigned int, int>& values);
	float computeConditionalProbability(const std::vector<unsigned int>& nodesNominator,const std::vector<unsigned int>& nodesDenominator,
		 std::unordered_map<unsigned int, int>& valuesNominator, std::unordered_map<unsigned int, int>& valuesDenominator);
	std::pair<float,std::vector<std::string>> maxSearch(const std::vector<unsigned int> queryNodes);
	float calculateLikelihoodOfTheData(const Matrix<int>& obs) const;

	private:
	std::vector<unsigned int> createFactorisation(const std::vector<unsigned int>& queryNodes);
	std::unordered_map<unsigned int, std::vector<int>> assignValues(const std::vector<unsigned int>& factorisation, std::unordered_map<unsigned int, int>& values);
	std::vector<std::vector<int>> enumerate(const std::vector<unsigned int>& factorisation, std::unordered_map<unsigned int, std::vector<int>>& valueAssignment);
	int getPosition(unsigned int id, const std::vector<unsigned int>& factorisation) const;
	int getParentValues(const Node& n, const std::vector<unsigned int>& factorisation, const std::vector<int>& assignment);
	int getParentValues(const Node& n, const Matrix<int>& obs,
	                    unsigned int index) const;
	float computeFullySpecifiedProbabilityDistribution(const std::vector<unsigned int>& factorisation, const std::vector<std::vector<int>>& combinations);
	
	Network& network_;	
};

#endif
