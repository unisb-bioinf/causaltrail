#ifndef PROBABILITYHANDLER_H
#define PROBABILITYHANDLER_H

#include "Network.h"
#include "Factor.h"

class ProbabilityHandler
{
	public:
	/**ProbabilityHandler
	 *
	 * @return ProbabilityHandler object
	 *
	 * Default Constructor
	 */
	ProbabilityHandler();

	/**ProbabilityHandler
	 *
	 * @param network, a reference to the network 
	 *
	 * @return ProbabilityHandler object
	 *
	 * This objects performs various kinds of probability calculations
	 */
	ProbabilityHandler(Network& network);

	/**computeTotalProbability
	 *
	 * @param nodeID, identifier of the node in focus
	 * @param value, value for whom the probability should be calculated
	 *
	 * @return total probability that the given value can be observerd in the given node
	 *
	 */
	float computeTotalProbability(int nodeID, int value);
	
	/**computeTotalProbabilityNormalized
	 *
	 * @param nodeID, identifier of the node in focus
	 * @param value, value for whom the probability should be calculated
	 *
	 * @return total probability that the given value can be observerd in the given node, normalized to 1.0
	 *
	 */
	float computeTotalProbabilityNormalized(int nodeID, int value);

	/**computeJointProbabilityUsingVariableElimination
	 *
	 * @param nodes, vector of node identifiers for whom the joint probability should be calculated
	 * @param values, vector of values for those nodes
	 *
	 * @return the joint probability, calculated using variable elimination
	 *
	 *
	 */
	float computeJointProbabilityUsingVariableElimination(
	    const std::vector<unsigned int>& nodes, const std::vector<int>& values);

	/**computeConditionalProbability
	 *
	 * @param nodesNonIntervention, vector containing the identifiers of the query nodes
	 * @param nodesCondition, vector containing the evidence nodes
	 * @param valuesNonIntervention, vector containing the values for the query nodes
	 * @param valuesCondition, vector containing the evidence values
	 *
	 * @return the conditionalProbability, calculated using variable elimination, normalized to 1.0
	 *
	 */
	float computeConditionalProbability(
	    const std::vector<unsigned int>& nodesNonIntervention,
	    const std::vector<unsigned int>& nodesCondition,
	    const std::vector<int>& valuesNonIntervention,
	    const std::vector<int>& valuesCondition);

	/**maxSearch
	 *
	 * @param queryNodes, vector containing the identifiers of the query nodes
	 * @param conditionNodes, vector containing the identifiers of the evidence nodes (obtional)
	 * @param conditionValues, vector containing the values for the evidence nodes (obtional)
	 *
	 * @return a pair of the MAP assignment for the query nodes, and the corresponding probability
	 *
	 */
	std::pair<float, std::vector<std::string>>
	maxSearch(const std::vector<unsigned int>& queryNodes,
	          const std::vector<unsigned int>& conditionNodes,
	          const std::vector<int>& conditionValues);

	/**calculateLikelihoodOfTheData
	 *
	 * @param obs, the observation matrix containing the discretised observations
	 *
	 * @return the log likelihood of the data
	 *
	 */
	float calculateLikelihoodOfTheData(const Matrix<int>& obs) const;

	private:

	/**createFactorisation
	 *
	 * @param queryNodes, vector of all node identifiers in the query
	 *
	 * @return list of all node identifiers which have to be considered for calculating the user query
	 *
	 * The procedure performs DFS for all query nodes to generate the factorisation
	 */
	std::vector<unsigned int>
	createFactorisation(const std::vector<unsigned int>& queryNodes);

	/**assignValues
	 *
	 * @param factorisation, factorisation of the query nodes
	 * @param values, vector containing known node values
	 *
	 * @return A vector of vector containing possible node value. The index of the outer vector
	 * equals the node identifier for whom the inner vector reflects the possible values
	 *
	 */
	std::vector<std::vector<int>>
	assignValues(const std::vector<unsigned int>& factorisation,
	             const std::vector<int>& values);

	/**enumerate
	 *
	 * @param factorisation, factorisation of the query nodes
	 * @param valueAssignment, value assignment produced by the procedure assignValues
	 *
	 * @return An enumeration of all possible value combinations for the nodes
	 * in the factorisation. This uses the class Combinations.
	 *
	 *
	 */
	std::vector<std::vector<int>>
	enumerate(const std::vector<unsigned int>& factorisation,
	          const std::vector<std::vector<int>>& valueAssignment);

	/**getParentValues
	 *
	 * @param n,
	 * @param obs,
	 * @param index,
	 *
	 * @return
	 *
	 *
	 */
	int getParentValues(const Node& n, const Matrix<int>& obs,
	                    unsigned int index) const;

	/**getResult
	 *
	 * @param factorlist, the vector of factors used in variable elimination
	 *
	 * @return the resulting probability for a calculation using variable elimination
	 *
	 * This method is used if joint probabilities are computed
	 */
	float getResult(std::vector<Factor>& factorlist);
	
	/**getResult
	 *
	 * @param factorlist, the vector of factors used in variable elimination
	 * @param values, observed values for the evidence nodes
	 *
	 * @return the resulting probability for a calculation using variable elimination
	 *
	 * This method is used if conditional probabilities are computed
	 */
	float getResult(std::vector<Factor>& factorlist,
	                const std::vector<int>& values);

	/**createFactorList
	 *
	 * @param factorisation, factorisation produced for the query nodes
	 * @param values, vector containig the values entered by the user
	 *
	 * @return a vector of factors for all nodes in the factorisation with respect
	 * to the given values
	 *
	 */
	std::vector<Factor>
	createFactorList(const std::vector<unsigned int>& factorisation,
	                 const std::vector<int>& values) const;

	/**getOrdering
	 *
	 * @param factorisation, factorisation produced for the query nodes
	 * @param queryNodes, vector containing identifiers of the query nodes
	 *
	 * @return a elimination ordering (for joint probabilities)
	 *
	 */
	std::vector<unsigned int>
	getOrdering(const std::vector<unsigned int>& factorisation,
	            const std::vector<unsigned int>& queryNodes) const;

	/**getOrdering
	 *
	 * @param factorisation, factorisation produced for the query nodes
	 * @param conditionNodes, vector containing the identifiers of the evidence nodes
	 * @param nonInternventionNodes, vector containing the identifiers of the none evidence nodes
	 *
	 * @return a elimination ordering (for conditional probabilities)
	 *
	 */
	std::vector<unsigned int>
	getOrdering(const std::vector<unsigned int>& factorisation,
	            const std::vector<unsigned int>& conditionNodes,
	            const std::vector<unsigned int>& nonInterventionNodes) const;

	/**eliminate
	 *
	 * @param factorlist, vector of factors
	 * @param values, vector of known values
	 * @param nonInterventionValues, vector of values for non evidence nodes
	 *
	 * Performs the elimination operation using the product and sumOut
	 * methods in the class Factor
	 */
	void eliminate(const unsigned int id, std::vector<Factor>& factorlist,
	               const std::vector<int>& values,
	               const std::vector<int>& nonInterventionValues);

	//A reference to the network
	Network& network_;
};

#endif
