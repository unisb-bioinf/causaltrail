#ifndef PROBABILITYHANDLER_H
#define PROBABILITYHANDLER_H

#include "Network.h"
#include "Factor.h"
#include <boost/algorithm/string.hpp>

class ProbabilityHandler
{
	public:
	ProbabilityHandler();
	ProbabilityHandler(Network& network);
	float computeTotalProbability(int nodeID, int value);
	float computeTotalProbabilityNormalized(int nodeID, int value);
	float computeJointProbability(const std::vector<unsigned int>& nodes,
	                              const std::vector<int>& values);
	float computeJointProbabilityUsingVariableElimination(
	    const std::vector<unsigned int>& nodes, const std::vector<int>& values);
	float computeConditionalProbability(
	    const std::vector<unsigned int>& nodesNominator,
	    const std::vector<unsigned int>& nodesDenominator,
	    const std::vector<int>& valuesNominator,
	    const std::vector<int>& valuesDenominator);
	std::pair<float, std::vector<std::string>>
	maxSearch(const std::vector<unsigned int>& queryNodes);
	float calculateLikelihoodOfTheData(const Matrix<int>& obs) const;

	private:
	std::vector<unsigned int>
	createFactorisation(const std::vector<unsigned int>& queryNodes);
	std::vector<std::vector<int>>
	assignValues(const std::vector<unsigned int>& factorisation,
	             const std::vector<int>& values);
	std::vector<std::vector<int>> enumerate(
	    const std::vector<unsigned int>& factorisation,
	    const std::vector<std::vector<int>>& valueAssignment);
	int getParentValues(const Node& n,
	                    const std::vector<unsigned int>& factorisation,
	                    const std::vector<int>& assignment);
	int getParentValues(const Node& n, const Matrix<int>& obs,
	                    unsigned int index) const;
	float computeFullySpecifiedProbabilityDistribution(
	    const std::vector<unsigned int>& factorisation,
		const std::pair<std::vector<unsigned int>, std::vector<unsigned int>>& nodePair,                        
	    const std::vector<std::vector<int>>& combinations);
	float getResult(std::vector<Factor>& factorlist);
	std::pair<std::vector<unsigned int>,std::vector<unsigned int>>
	getNoneSummationNodes(const std::vector<unsigned int>& queryNodes,
	                  const std::vector<unsigned int>& factorisation);
	std::vector<Factor> createFactorList(const std::vector<unsigned int>& factorisation, const std::vector<int>& values) const;
	std::vector<unsigned int> getOrdering(const std::vector<unsigned int>& factorisation,const std::vector<unsigned int>& queryNodes) const;
	void eliminate(const unsigned int id, std::vector<Factor>& factorlist);



	Network& network_;
};

#endif
