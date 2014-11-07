#include "ProbabilityHandler.h"
#include "Combinations.h"

/*ProbabilityHandler
 *
 * @param network the network object we are working with
 *
 * @return ProbabilityHandler object
 *
 * This class offers methods to calculat probabilities in the CBN
 */
ProbabilityHandler::ProbabilityHandler(Network& network)
	:network_(network)	{
}


/*computeTotalProbability
 *
 * @param node
 * @param value
 *
 * @return total probability for the specified value
 *
 * This method computes recursively the total probability for a given value at a specified node
 */
float ProbabilityHandler::computeTotalProbability(const Node& node, std::string value){
    //Get Parents
    const auto& parentIDs = node.getParents();
    const auto& probMatrix = node.getProbabilityMatrix();
    //Check Existens
    if (parentIDs.size()!=0){
	    //Yes -> Call recursively for all parent values
        float result = 0.0f;
        for (unsigned int row=0; row<probMatrix.getRowCount(); row++){
            std::vector<std::string> values;
			boost::algorithm::split(values,probMatrix.getRowNames()[row],boost::algorithm::is_any_of(","));
            float temp=1.0f;
            for (unsigned int index=0; index<values.size();index++){
                temp*=computeTotalProbability(network_.getNode(parentIDs[index]), values[index]);
            }
			unsigned int column = probMatrix.findCol(value);
			if (column != -1){
	            result+=(temp*probMatrix(probMatrix.findCol(value), row)); 
			}
			else {
			throw std::invalid_argument("The current node does contain the query value");
			}
        }
        return result;
    }

    //No -> return value
    return probMatrix(probMatrix.findCol(value),0);
}

/*createFactorisation
 *
 * @param std::vector<Node&> queryNodes
 * 
 * @result std::vector<Node&> factorisation
 *
 * Computes a factorisation of the given jointProbabilityQuery
 */
std::vector<unsigned int> ProbabilityHandler::createFactorisation(std::vector<unsigned int>& queryNodes){
	std::vector<unsigned int> visitedNodes;
	network_.setAllUnvisited();
	for (auto& id : queryNodes){
		network_.performDFS(id,visitedNodes);
	}
	for (auto id : visitedNodes){
		Node& node = network_.getNode(id);
		std::cout<<node.getName()<<std::endl;
		}
	return visitedNodes;
}

/*assignValues
 *
 * @param std::vector<int&>& factorisation
 * @param std::unordered_map<int, std::string>& values for the query nodes
 * 
 * @result a total value assignment
 *
 * Description Computes the value assignment for the factorisation
 */
std::unordered_map<unsigned int, std::vector<std::string>> ProbabilityHandler::assignValues(std::vector<unsigned int>& factorisation, std::unordered_map<unsigned int, std::string>& values){
	std::unordered_map<unsigned int, std::vector<std::string>> possibleValueMap;
	for (unsigned int id: factorisation){
		if (values.find(id)!=values.end()){
			possibleValueMap[id]={values[id]};
		}
		else{
			possibleValueMap[id]=network_.getNode(id).getValueNamesProb();
		}
	}
	return possibleValueMap;
}



/*enumerate
 *
 * @param std::vector<int>& factorisation
 * @param std::unordered_map<unsigned int, std::vector<std::string>> valueAssignment
 * 
 * @result std::vector<std::vector<std::string>> combinations
 *
 * Computes all valid combinations of node values
 */
std::vector<std::vector<std::string>> ProbabilityHandler::enumerate(std::vector<unsigned int>& factorisation, std::unordered_map<unsigned int, std::vector<std::string>> valueAssignment){
	Combinations<std::string> com = Combinations<std::string> (factorisation, valueAssignment);
	com.createCombinations(0);
	return com.getResult();
}

/*getPosition
 *
 * @param std::vector<Node&>& factorisation
 * @param std::vector<std::vector<std::string>>& combinations
 *
 * @result float probability
 *
 * Computes the joint probability
 */
int ProbabilityHandler::getPosition(unsigned int& id, std::vector<unsigned int>& factorisation){
	for (int i=0; i<factorisation.size(); i++){
		if (factorisation[i]==id){
			return i;
		}
	}
	throw std::invalid_argument("Element not found in factorisation");
}

/*getParentValues
 *
 * @param
 * @param
 *
 * @result float probability
 *
 * Computes the joint probability
 */
std::string ProbabilityHandler::getParentValues(Node& n, unsigned int index, std::vector<unsigned int>& factorisation, std::vector<std::string>& assignment){
	std::string temp="";
	std::vector<unsigned int> parents = n.getParents();
	if (parents.empty()){
		return "1";
	}
	for(auto parent: parents){
		int position = getPosition(parent, factorisation);
		temp+=assignment[position]+",";		
	}
	temp.erase(temp.end()-1);
	return temp;
}

/*computeFullySpecifiedPRobabilityDistribution
 *
 * @param std::vector<Node&>& factorisation
 * @param std::vector<std::vector<std::string>>& combinations
 *
 * @result float probability
 *
 * Computes the joint probability
 */
float ProbabilityHandler::computeFullySpecifiedProbabilityDistribution(std::vector<unsigned int>& factorisation, std::vector<std::vector<std::string>>& combinations){
	float result = 0.0f;
	for (auto& assignment : combinations){
		float intermediatResult= 1.0f;
		for (unsigned int index = 0; index<factorisation.size(); index++){
			std::string nodeValue=assignment[index];

			Node& n = network_.getNode(factorisation[index]);
			std::string parentValues=getParentValues(n, index, factorisation, assignment);

			intermediatResult*=n.getProbability(nodeValue, parentValues);
		}
		result+=intermediatResult;
	}
	return result;
}

/*computeJointProbability
 *
 * @param std::vector<Node&> a vector of Node references
 * @param std::unordered_map<int,std::string> a map from node identifiers to node values
 *
 * @return float the joint probability for the given nodes and values
 */
float ProbabilityHandler::computeJointProbability(std::vector<unsigned int>& queryNodes, std::unordered_map<unsigned int, std::string>& values){
	//Generate the factorisation of the joint probability distribution
	std::vector<unsigned int> factorisation=createFactorisation(queryNodes);

	//Assign possible values to the node identifiers (needed for the creation of the combinations)
	std::unordered_map<unsigned int, std::vector<std::string>> valueAssignment = assignValues(factorisation, values);

	//Enumerate all possible value assignments
	std::vector<std::vector<std::string>> combinations = enumerate(factorisation, valueAssignment);	
	
	//Compute the joint probability by summing up all combinations
	return computeFullySpecifiedProbabilityDistribution(factorisation, combinations);
}

/*computeConditionalProbability
 *
 * @param std::vector<int> a vector of node identifieres for the nominator
 * @param std::vector<int> a vector of node identifieres for the denominator
 * @param std::unordered_map<int,std::string> a map from node identifiers to node values
 *
 * @return float the conditional probability for the given nodes and values
 */
float ProbabilityHandler::computeConditionalProbability(std::vector<unsigned int>& nodesNominator, std::vector<unsigned int>& nodesDenominator,std::unordered_map<unsigned int, std::string>& valuesNominator, std::unordered_map<unsigned int, std::string>& valuesDenominator){
	return computeJointProbability(nodesNominator, valuesNominator) / computeJointProbability(nodesDenominator, valuesDenominator);
}

/*maxSearch
 *
 * @param std::vector<int> a vector of node identifiers
 *
 * @return std:vector<std::string> a vector of values
 */
