#include "ProbabilityHandler.h"
#include "Combinations.h"

ProbabilityHandler::ProbabilityHandler(Network& network)
	:network_(network)	{
}

float ProbabilityHandler::computeTotalProbabilityNormalized(int nodeID, int index){
    //Get Parents
	const Node& node = network_.getNode(nodeID);
    const auto& parentIDs = node.getParents();
    const auto& probMatrix = node.getProbabilityMatrix();
    //Check Existens
    if (parentIDs.size()!=0){
	    //Yes -> Call recursively for all parent values
        float queryResult = 0.0f;
        for (unsigned int row=0; row<probMatrix.getRowCount(); row++){
            float temp=1.0f;
            for (unsigned int index2=0; index2<parentIDs.size();index2++){
                temp*=computeTotalProbability(parentIDs[index2], network_.reverseFactor(node,parentIDs[index2],row));
            }
			if (index != -1){
	            queryResult+=(temp*probMatrix(index, row)); 
			}
			else {
			throw std::invalid_argument("The current node does contain the query value");
			}
        }
		float norm = 0.0f;
		for (unsigned int col=0; col<probMatrix.getColCount(); col++){
			norm +=computeTotalProbability(nodeID,col);
		}
        return queryResult/norm;
    }
    //No -> return value
    return probMatrix(index,0);
}

float ProbabilityHandler::computeTotalProbability(int nodeID, int index){
    //Get Parents
	const Node& node = network_.getNode(nodeID);
    const auto& parentIDs = node.getParents();
    const auto& probMatrix = node.getProbabilityMatrix();
    //Check Existens
    if (parentIDs.size()!=0){
	    //Yes -> Call recursively for all parent values
        float result = 0.0f;
        for (unsigned int row=0; row<probMatrix.getRowCount(); row++){
            float temp=1.0f;
            for (unsigned int index2=0; index2<parentIDs.size();index2++){
                temp*=computeTotalProbability(parentIDs[index2], network_.reverseFactor(node,parentIDs[index2],row));
            }
			if (index != -1){
	            result+=(temp*probMatrix(index, row)); 
			}
			else {
			throw std::invalid_argument("The current node does not contain the query value");
			}
        }
        return result;
    }
    //No -> return value
    return probMatrix(index,0);
}

std::vector<unsigned int> ProbabilityHandler::createFactorisation(const std::vector<unsigned int>& queryNodes){
	std::vector<unsigned int> visitedNodes;
	network_.setAllUnvisited();
	for (auto& id : queryNodes){
		network_.performDFS(id,visitedNodes);
	}
	return visitedNodes;
}

std::unordered_map<unsigned int, std::vector<int>> ProbabilityHandler::assignValues(const std::vector<unsigned int>& factorisation,  std::unordered_map<unsigned int, int>& values){
	std::unordered_map<unsigned int, std::vector<int>> possibleValueMap;
	for (unsigned int id: factorisation){
		if (values.find(id)!=values.end()){
			possibleValueMap[id]={values[id]};
		}
		else{
			possibleValueMap[id]=network_.getNode(id).getUniqueValuesExcludingNA();
		}
	}
	return possibleValueMap;
}

std::vector<std::vector<int>> ProbabilityHandler::enumerate(const std::vector<unsigned int>& factorisation, std::unordered_map<unsigned int, std::vector<int>>& valueAssignment){
	Combinations<int> com = Combinations<int> (factorisation, valueAssignment);
	com.createCombinations(0);
	return com.getResult();
}

int ProbabilityHandler::getPosition(unsigned int& id, const std::vector<unsigned int>& factorisation){
	for (int i=0; i<factorisation.size(); i++){
		if (factorisation[i]==id){
			return i;
		}
	}
	throw std::invalid_argument("Element not found in factorisation");
}

int ProbabilityHandler::getParentValues(const Node& n, const std::vector<unsigned int>& factorisation, const std::vector<int>& assignment){
	int pos = 0;
	const std::vector<unsigned int>& parents = n.getParents();
	if (parents.empty()){
		return 0;
	}
	for(auto parent: parents){
		int position = getPosition(parent, factorisation);
		pos+=network_.computeFactor(n,parent)*assignment[position];
	}
	return pos;
}

float ProbabilityHandler::computeFullySpecifiedProbabilityDistribution(const std::vector<unsigned int>& factorisation, const std::vector<std::vector<int>>& combinations){
	float result = 0.0f;
	for (auto& assignment : combinations){
		float intermediatResult= 1.0f;
		for (unsigned int index = 0; index<factorisation.size(); index++){
			int col=assignment[index];

			const Node& n = network_.getNode(factorisation[index]);
			int  row=getParentValues(n, factorisation, assignment);

			intermediatResult*=n.getProbability(col, row);
		}
		result+=intermediatResult;
	}
	return result;
}

float ProbabilityHandler::computeJointProbabilityWithoutNormalization(const std::vector<unsigned int>& queryNodes, std::unordered_map<unsigned int, int>& values){
	//Generate the factorisation of the joint probability distribution
	std::vector<unsigned int> factorisation=createFactorisation(queryNodes);

	//Assign possible values to the node identifiers (needed for the creation of the combinations)
	std::unordered_map<unsigned int, std::vector<int>> valueAssignment = assignValues(factorisation, values);

	//Enumerate all possible value assignments
	std::vector<std::vector<int>> combinations = enumerate(factorisation, valueAssignment);	
	//Enumerate normalization

	//Compute the joint probability by summing up all combinations
	return computeFullySpecifiedProbabilityDistribution(factorisation, combinations);
}

float ProbabilityHandler::computeJointProbability(const std::vector<unsigned int>& queryNodes, std::unordered_map<unsigned int, int>& values){
	//Generate the factorisation of the joint probability distribution
	std::vector<unsigned int> factorisation=createFactorisation(queryNodes);

	//Assign possible values to the node identifiers (needed for the creation of the combinations)
	std::unordered_map<unsigned int, int> emptyMap;
	std::unordered_map<unsigned int, std::vector<int>> valueAssignment = assignValues(factorisation, values);
	std::unordered_map<unsigned int, std::vector<int>> fullAssignment = assignValues(factorisation, emptyMap);

	//Enumerate all possible value assignments
	std::vector<std::vector<int>> combinations = enumerate(factorisation, valueAssignment);	
	//Enumerate normalization
	std::vector<std::vector<int>> normalizations = enumerate(factorisation, fullAssignment);

	//Compute the joint probability by summing up all combinations
	return computeFullySpecifiedProbabilityDistribution(factorisation, combinations)/computeFullySpecifiedProbabilityDistribution(factorisation, normalizations);
}

float ProbabilityHandler::computeConditionalProbability(const std::vector<unsigned int>& nodesNominator, const std::vector<unsigned int>& nodesDenominator,std::unordered_map<unsigned int, int>& valuesNominator, std::unordered_map<unsigned int, int>& valuesDenominator){
	return computeJointProbability(nodesNominator, valuesNominator) / computeJointProbability(nodesDenominator, valuesDenominator);
}

std::pair<float,std::vector<std::string>> ProbabilityHandler::maxSearch(const std::vector<unsigned int> queryNodes){
	std::unordered_map<unsigned int, int> temp;
	//Assign Values
    std::unordered_map<unsigned int, std::vector<int>> valueAssignment = assignValues(queryNodes,temp);
	//Enumerate all possible value assignments
	std::vector<std::vector<int>> combinations = enumerate(queryNodes, valueAssignment);
	//Search most probable assignment	
	int index=0;
	int maxIndex=0;
	float maxprob=0.0f;
	for (auto& com: combinations){
		for (int index = 0; index<queryNodes.size(); index++){
			temp[queryNodes[index]]=com[index];
		}
		float prob = computeJointProbability(queryNodes,temp);
		if (prob > maxprob){
			maxprob=prob;
			maxIndex=index;
		}
		index++;
	}
	std::vector<std::string> resultNames;

	for (int index = 0; index<queryNodes.size(); index++){
		int value = combinations[maxIndex][index];
		const Node& node = network_.getNode(queryNodes[index]);
		resultNames.push_back(node.getValueNamesProb()[value]);
	}
	return std::make_pair(maxprob,resultNames);
}
