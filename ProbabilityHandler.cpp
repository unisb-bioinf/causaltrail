#include "ProbabilityHandler.h"
#include "Combinations.h"

ProbabilityHandler::ProbabilityHandler(Network& network) : network_(network) {}

float ProbabilityHandler::computeTotalProbabilityNormalized(int nodeID,
                                                            int index)
{
	// Get Parents
	const Node& node = network_.getNode(nodeID);
	const auto& parentIDs = node.getParents();
	const auto& probMatrix = node.getProbabilityMatrix();
	// Check Existens
	if(node.getNumberOfParents() != 0) {
		// Yes -> Call recursively for all parent values
		float queryResult = 0.0f;
		for(unsigned int row = 0; row < probMatrix.getRowCount(); row++) {
			float temp = 1.0f;
			for(unsigned int index2 = 0; index2 < node.getNumberOfParents();
			    index2++) {
				temp *= computeTotalProbability(
				    parentIDs[index2],
				    network_.reverseFactor(node, parentIDs[index2], row));
			}
			if(index != -1) {
				queryResult += (temp * probMatrix(index, row));
			} else {
				throw std::invalid_argument(
				    "The current node does contain the query value");
			}
		}
		float norm = 0.0f;
		for(unsigned int col = 0; col < probMatrix.getColCount(); col++) {
			norm += computeTotalProbability(nodeID, col);
		}
		return queryResult / norm;
	}
	// No -> return value
	return probMatrix(index, 0);
}

float ProbabilityHandler::computeTotalProbability(int nodeID, int index)
{
	// Get Parents
	const Node& node = network_.getNode(nodeID);
	const auto& parentIDs = node.getParents();
	const auto& probMatrix = node.getProbabilityMatrix();
	// Check Existens
	if(node.getNumberOfParents() != 0) {
		// Yes -> Call recursively for all parent values
		float result = 0.0f;
		for(unsigned int row = 0; row < probMatrix.getRowCount(); row++) {
			float temp = 1.0f;
			for(unsigned int index2 = 0; index2 < node.getNumberOfParents();
			    index2++) {
				temp *= computeTotalProbability(
				    parentIDs[index2],
				    network_.reverseFactor(node, parentIDs[index2], row));
			}
			if(index != -1) {
				result += (temp * probMatrix(index, row));
			} else {
				throw std::invalid_argument(
				    "The current node does not contain the query value");
			}
		}
		return result;
	}
	// No -> return value
	return probMatrix(index, 0);
}

std::vector<unsigned int> ProbabilityHandler::createFactorisation(
    const std::vector<unsigned int>& queryNodes)
{
	std::vector<unsigned int> visitedNodes;
	network_.setAllUnvisited();
	for(auto& id : queryNodes) {
		network_.performDFS(id, visitedNodes);
	}
	return visitedNodes;
}

std::vector<std::vector<int>>
ProbabilityHandler::assignValues(const std::vector<unsigned int>& factorisation,
                                 const std::vector<int>& values)
{
	std::vector<std::vector<int>> possibleValueMap(values.size(), {-1});
	for(unsigned int id : factorisation) {
		if(values[id] != -1) {
			possibleValueMap[id] = {values[id]};
		} else {
			possibleValueMap[id] =
			    network_.getNode(id).getUniqueValuesExcludingNA();
		}
	}
	return possibleValueMap;
}

std::vector<std::vector<int>> ProbabilityHandler::enumerate(
    const std::vector<unsigned int>& factorisation,
    const std::vector<std::vector<int>>& valueAssignment)
{
	Combinations<int> com = Combinations<int>(factorisation, valueAssignment);
	com.createCombinations(0);
	return com.getResult();
}

int ProbabilityHandler::getParentValues(
    const Node& n, const std::vector<unsigned int>& factorisation,
    const std::vector<int>& assignment)
{
	int pos = 0;
	const std::vector<unsigned int>& parents = n.getParents();
	if(parents.empty()) {
		return 0;
	}
	for(auto parent : parents) {
		pos += network_.computeFactor(n, parent) * assignment[parent];
	}
	return pos;
}

int ProbabilityHandler::getParentValues(const Node& n, const Matrix<int>& obs,
                                        unsigned int sample) const
{
	const auto& parents = n.getParents();

	int pos = 0;
	for(auto pi : parents) {
		const Node& parent = network_.getNode(pi);
		pos += network_.computeFactor(n, pi) *
		       obs(sample, parent.getObservationRow());
	}

	return pos;
}

float ProbabilityHandler::computeFullySpecifiedProbabilityDistribution(
    const std::vector<unsigned int>& factorisation,
    const std::vector<std::vector<int>>& combinations)
{
	float result = 0.0f;
	for(auto& assignment : combinations) {
		float intermediatResult = 1.0f;
		for(unsigned int index = 0; index < factorisation.size(); index++) {
			int col = assignment[factorisation[index]];

			const Node& n = network_.getNode(factorisation[index]);
			int row = getParentValues(n, factorisation, assignment);

			intermediatResult *= n.getProbability(col, row);
		}
		result += intermediatResult;
	}
	return result;
}

float ProbabilityHandler::calculateLikelihoodOfTheData(const Matrix<int>& obs)
    const
{
	float prob = 0.0f;
	for(unsigned int sample = 0; sample < obs.getColCount(); sample++) {
		if(!obs.containsElement(0, sample, -1)) {
			float intermediateResult = 1.0f;
			for(unsigned int index = 0; index < obs.getRowCount(); ++index) {
				const Node& n = network_.getNode(obs.getRowNames()[index]);
				int row = getParentValues(n, obs, sample);
				intermediateResult *=
				    n.getProbability(obs(sample, n.getObservationRow()), row);
			}

			prob += intermediateResult;
		}
	}
	return log(prob);
}

float ProbabilityHandler::computeJointProbability(
    const std::vector<unsigned int>& queryNodes, const std::vector<int>& values)
{
	std::cout << "New" << std::endl;
	// Generate the factorisation of the joint probability distribution
	std::vector<unsigned int> factorisation = createFactorisation(queryNodes);
	// Assign possible values to the node identifiers (needed for the creation
	// of the combinations)
	std::vector<int> emptyVec(network_.size(), -1);

	std::vector<std::vector<int>> valueAssignment =
	    assignValues(factorisation, values);

	std::vector<std::vector<int>> fullAssignment =
	    assignValues(factorisation, emptyVec);

	// Enumerate all possible value assignments
	std::vector<std::vector<int>> combinations =
	    enumerate(factorisation, valueAssignment);

	// Enumerate normalization
	std::vector<std::vector<int>> normalizations =
	    enumerate(factorisation, fullAssignment);

	// Compute the joint probability by summing up all combinations
	return computeFullySpecifiedProbabilityDistribution(factorisation,
	                                                    combinations) /
	       computeFullySpecifiedProbabilityDistribution(factorisation,
	                                                    normalizations);
}

float ProbabilityHandler::computeConditionalProbability(
    const std::vector<unsigned int>& nodesNominator,
    const std::vector<unsigned int>& nodesDenominator,
    const std::vector<int>& valuesNominator,
    const std::vector<int>& valuesDenominator)
{
	return computeJointProbability(nodesNominator, valuesNominator) /
	       computeJointProbability(nodesDenominator, valuesDenominator);
}

std::pair<float, std::vector<std::string>>
ProbabilityHandler::maxSearch(const std::vector<unsigned int>& queryNodes)
{
	std::vector<int> emptyValues(network_.size(), -1);
	std::vector<std::vector<int>> queryAssignment =
	    assignValues(queryNodes, emptyValues);
	std::vector<std::vector<int>> combinations =
	    enumerate(queryNodes, queryAssignment);

	int index = 0;
	int maxIndex = 0;
	float maxprob = 0.0f;

	for(auto& com : combinations) {
		for(auto& id : queryNodes) {
			emptyValues[id] = com[id];
		}
		float prob = computeJointProbability(queryNodes, emptyValues);
		if(prob > maxprob) {
			maxprob = prob;
			maxIndex = index;
		}
		index++;
	}
	std::vector<std::string> resultNames;

	for(unsigned int index = 0; index < queryNodes.size(); index++) {
		int value = combinations[maxIndex][index];
		const Node& node = network_.getNode(queryNodes[index]);
		resultNames.push_back(node.getValueNamesProb()[value]);
	}
	return std::make_pair(maxprob, resultNames);
}
