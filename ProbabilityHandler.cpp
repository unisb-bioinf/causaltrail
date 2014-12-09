#include "ProbabilityHandler.h"
#include "Combinations.h"

ProbabilityHandler::ProbabilityHandler(Network& network) : network_(network) {}

float ProbabilityHandler::computeTotalProbabilityNormalized(int nodeID,
                                                            int index)
{
	if(index == -1) {
		throw std::invalid_argument(
		    "The current node does contain the query value");
	}

	// Get Parents
	const Node& node = network_.getNode(nodeID);
	const auto& parentIDs = node.getParents();
	const auto& probMatrix = node.getProbabilityMatrix();
	// Check Existens
	if(node.getNumberOfParents() != 0) {
		// Yes -> Call recursively for all parent values
		float queryResult = 0.0f;

		for(unsigned int row = 0; row < probMatrix.getRowCount(); row++) {
			if(node.isCalculated(index, row)) {
				queryResult += node.getCalculatedValue(index, row);
			} else {
				float temp = 1.0f;
				for(unsigned int index2 = 0; index2 < node.getNumberOfParents();
				    index2++) {
					temp *= computeTotalProbability(
					    parentIDs[index2],
					    network_.reverseFactor(node, parentIDs[index2], row));
				}
				queryResult += (temp * probMatrix(index, row));
			}
		}
		float norm = 0.0f;
		unsigned int queryCol = index;
		for(unsigned int col = 0; col < probMatrix.getColCount(); col++) {
			if(col != queryCol) {
				norm += computeTotalProbability(nodeID, col);
			} else {
				norm += queryResult;
			}
		}
		return queryResult / norm;
	}
	// No -> return value
	return probMatrix(index, 0);
}

float ProbabilityHandler::computeTotalProbability(int nodeID, int index)
{
	if(index == -1) {
		throw std::invalid_argument(
		    "The current node does not contain the query value");
	}
	// Get Parents
	Node& node = network_.getNode(nodeID);
	const auto& parentIDs = node.getParents();
	const auto& probMatrix = node.getProbabilityMatrix();
	// Check Existens
	if(node.getNumberOfParents() != 0) {
		// Yes -> Call recursively for all parent values
		float result = 0.0f;
		for(unsigned int row = 0; row < probMatrix.getRowCount(); row++) {
			if(node.isCalculated(index, row)) {
				result += node.getCalculatedValue(index, row);
			} else {
				float temp = 1.0f;
				for(unsigned int index2 = 0; index2 < node.getNumberOfParents();
				    index2++) {
					temp *= computeTotalProbability(
					    parentIDs[index2],
					    network_.reverseFactor(node, parentIDs[index2], row));
				}
				float tempResult = temp * probMatrix(index, row);
				node.setCalculatedValue(tempResult, index, row);
				result += tempResult;
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

float ProbabilityHandler::calculateLikelihoodOfTheData(const Matrix<int>& obs)
    const
{
	float prob = 0.0f;
	for(unsigned int sample = 0; sample < obs.getColCount(); sample++) {

		if(!obs.containsElement(0, sample, -1)) {
			float intermediateResult = 1.0f;

			for(const Node& n : network_.getNodes()) {
				int row = getParentValues(n, obs, sample);
				intermediateResult *=
				    n.getProbability(obs(sample, n.getObservationRow()), row);
			}

			prob += intermediateResult;
		}
	}
	return log(prob);
}

std::vector<Factor> ProbabilityHandler::createFactorList(
    const std::vector<unsigned int>& factorisation,
    const std::vector<int>& values) const
{
	std::vector<Factor> temp;
	temp.reserve(factorisation.size());
	for(auto& id : factorisation) {
		temp.push_back(Factor(network_.getNode(id), values));
	}
	return temp;
}

std::vector<unsigned int>
ProbabilityHandler::getOrdering(const std::vector<unsigned int>& factorisation,
                                const std::vector<unsigned int>& queryNodes)
    const
{
	auto temp = queryNodes;
	for(auto& id : factorisation) {
		if(std::find(queryNodes.begin(), queryNodes.end(), id) ==
		   queryNodes.end()) {
			temp.push_back(id);
		}
	}
	return temp;
}

std::vector<unsigned int> ProbabilityHandler::getOrdering(
    const std::vector<unsigned int>& factorisation,
    const std::vector<unsigned int>& conditionNodes,
    const std::vector<unsigned int>& nonInterventionNodes) const
{
	auto temp = conditionNodes;
	for(auto& id : factorisation) {
		if((std::find(conditionNodes.begin(), conditionNodes.end(), id) ==
		   conditionNodes.end()) and (std::find(nonInterventionNodes.begin(), nonInterventionNodes.end(), id) == nonInterventionNodes.end())) {
			temp.push_back(id);
		}
	}
	for(auto& id : nonInterventionNodes) {
		temp.push_back(id);
	}
	return temp;
}

void ProbabilityHandler::eliminate(const unsigned int id,
                                   std::vector<Factor>& factorlist,
                                   const std::vector<int>& values,
									const std::vector<int>& nonInterventionValues = {})
{
	std::vector<unsigned int> neededFactors;
	std::vector<std::vector<unsigned int>> neededFactorsIDs;
	for(unsigned int i = 0; i < factorlist.size(); i++) {
		Factor& f = factorlist[i];
		if(std::find(f.getIDs().begin(), f.getIDs().end(), id) !=
		   f.getIDs().end()) {
			neededFactors.push_back(i);
			neededFactorsIDs.push_back(f.getIDs());
		}
	}
	Factor tempFactor = factorlist[neededFactors[0]];
	if(neededFactors.size() > 1) {
		for(unsigned int i = 1; i < neededFactors.size(); i++) {
			tempFactor = tempFactor.product(factorlist[neededFactors[i]],network_,values);
		}
	}
	std::cout<<"Product"<<std::endl;
	std::cout<<tempFactor<<std::endl;
	if ((nonInterventionValues.empty()) or (values[id] != -1) or ((values[id] == -1) and (nonInterventionValues[id] == -1))){
		tempFactor = tempFactor.sumOut(id, network_, values);
	}
	for(auto& neededFactorID : neededFactorsIDs) {
		auto it = factorlist.begin();
		for(; it != factorlist.end();) {
			if(it->getIDs() == neededFactorID) {
				it = factorlist.erase(it);
			} else {
				++it;
			}
		}
	}
	std::cout<<"SumOut"<<std::endl;
	std::cout<<tempFactor<<std::endl;
	factorlist.push_back(tempFactor);
}

float ProbabilityHandler::getResult(std::vector<Factor>& factorlist)
{
	float prob = 1.0f;
	for(auto& f : factorlist) {
		prob *= f.getProbability(0);
	}
	return prob;
}


float ProbabilityHandler::getResult(std::vector<Factor>& factorlist, const std::vector<int>& values)
{
	for(auto& f : factorlist) {
		f.normalize();
	}
	std::cout<<factorlist[0]<<std::endl;
	float prob = 1.0f;
	for (auto& f: factorlist){
		prob *= f.getProbability(values);
	}
	return prob;
}



float ProbabilityHandler::computeJointProbabilityUsingVariableElimination(
    const std::vector<unsigned int>& queryNodes, const std::vector<int>& values)
{
	auto factorisation = createFactorisation(queryNodes);
	auto factorlist = createFactorList(factorisation, values);
	auto ordering = getOrdering(factorisation, queryNodes);
	for(auto& id : ordering) {
		eliminate(id, factorlist, values);
	}
	return getResult(factorlist);
}

float ProbabilityHandler::computeConditionalProbability(
    const std::vector<unsigned int>& nodesNonIntervention,
    const std::vector<unsigned int>& nodesCondition,
    const std::vector<int>& valuesNonIntervention,
    const std::vector<int>& valuesCondition)
{	
	auto allNodes = nodesNonIntervention;
	allNodes.insert(allNodes.end(), nodesCondition.begin(), nodesCondition.end());
	auto factorisation = createFactorisation(allNodes);
	auto factorlist = createFactorList(factorisation, valuesCondition);
	auto ordering = getOrdering(factorisation, nodesCondition, nodesNonIntervention);
	for (auto& id : ordering) {
		eliminate(id, factorlist, valuesCondition, valuesNonIntervention);
	}
	std::cout<<"Final List"<<std::endl;
	for (auto& f : factorlist){
		std::cout<<f<<std::endl;
	}
	return getResult(factorlist,valuesNonIntervention);
}

std::pair<float, std::vector<std::string>>
ProbabilityHandler::maxSearch(const std::vector<unsigned int>& queryNodes,
                              const std::vector<unsigned int>& conditionNodes,
                              const std::vector<int>& conditionValues)
{
	std::vector<int> emptyValues(network_.size(), -1);
	std::vector<std::vector<int>> queryAssignment =
	    assignValues(queryNodes, emptyValues);
	std::vector<std::vector<int>> combinations =
	    enumerate(queryNodes, queryAssignment);

	int index = 0;
	int maxIndex = 0;
	float maxprob = 0.0f;

	if(queryNodes.size() > 1) {
		for(auto& com : combinations) {
			float prob = 0.0f;
			for(auto& id : queryNodes) {
				emptyValues[id] = com[id];
			}
			if (conditionNodes.empty()) {
				prob = computeJointProbabilityUsingVariableElimination(
				    queryNodes, emptyValues);
			} else {
				prob = computeConditionalProbability(
				    queryNodes, conditionNodes, emptyValues,conditionValues);
			}
			if(prob > maxprob) {
				maxprob = prob;
				maxIndex = index;
			}
			index++;
		}
	} else {
		for(auto& com : combinations) {
			float prob = 0.0f;
			if(conditionNodes.empty()) {
				prob =
				    computeTotalProbabilityNormalized(queryNodes[0], com[queryNodes[0]]);
			} else {
				emptyValues[queryNodes[0]]=com[queryNodes[0]];
				prob = computeConditionalProbability(
				    {queryNodes[0]}, conditionNodes, emptyValues,
				    conditionValues);
			}
			if(prob > maxprob) {
				maxprob = prob;
				maxIndex = index;
			}
			index++;
		}
	}
	std::vector<std::string> resultNames;

	for(unsigned int index = 0; index < queryNodes.size(); index++) {
		int value = combinations[maxIndex][index];
		const Node& node = network_.getNode(queryNodes[index]);
		resultNames.push_back(node.getValueNamesProb()[value]);
	}
	return std::make_pair(maxprob, resultNames);
}
