#ifndef PROBABILITYHANDLER_H
#define PROBABILITYHANDLER_H

#include "Network.h"
#include <boost/algorithm/string.hpp>

class ProbabilityHandler{
	public:
	ProbabilityHandler(Network& network);
	float computeTotalProbability(const Node& node, std::string value);
	float computeTotalProbability(const Node& node, int value);
	float computeJointProbability(std::vector<unsigned int>& nodes, std::unordered_map<unsigned int,std::string>& values);
	float computeConditionalProbability(std::vector<unsigned int>& nodesNominator, std::vector<unsigned int>& nodesDenominator,
		 std::unordered_map<unsigned int, std::string>& valuesNominator, std::unordered_map<unsigned int, std::string>& valuesDenominator);
	private:
	std::vector<unsigned int> createFactorisation(std::vector<unsigned int>& queryNodes);
	std::unordered_map<unsigned int, std::vector<std::string>> assignValues(std::vector<unsigned int>& factorisation, std::unordered_map<unsigned int, std::string>& values);
	std::vector<std::vector<std::string>> enumerate(std::vector<unsigned int>& factorisation,std::unordered_map<unsigned int, std::vector<std::string>> valueAssignment);
	std::string getNodeValue(unsigned int index, std::vector<std::string>& assignment);
	int getPosition(unsigned int& id, std::vector<unsigned int>& factorisation);
	std::string getParentValues(Node& n, unsigned int index, std::vector<unsigned int>& factorisation, std::vector<std::string>& assignment);
	float getIntermediateResult(const Node&, const std::string& nodeValue, const std::string& parentValue);
	float computeFullySpecifiedProbabilityDistribution(std::vector<unsigned int>& factorisation, std::vector<std::vector<std::string>>& combinations);
	Network& network_;	
};

#endif
