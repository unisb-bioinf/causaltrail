#ifndef DATADISTRIBUTION_H 
#define DATADISTRIBUTION_H

#include"Network.h"
#include"Combinations.h"
#include<map>

class DataDistribution{
	public:
	/**DataDistribution
	 *
	 * @param network, A reference to a network
	 * @param observations, A reference to a Matrix of typ int containing the discretised observations
	 *
	 * @return DataDistribution object
	 *
	 */
	DataDistribution(Network& network, Matrix<int>& observations);

	/**assignObservationsToNodes
	 *
	 * This method initalises each node with the correct value and name vectors.
	 * It also creates Backups for later usage.
	 */
	void assignObservationsToNodes();

	/**distributeObservations
	 *
	 * Counts the number of observatoins of a given value,
 	 * and assigns it to the corresponding node, with respect
	 * to the values of its parents. It also initalizes a properly sized
	 * CPT.
	 */
	void distributeObservations();
	private:

	/**computeParentCombinations
	 *
	 * @param parents, a vector containing the node identifiers of the parents
	 *
	 * @return number of possible combinations of parent values
	 *
	 */
	int computeParentCombinations(std::vector<unsigned int> parents);

	/**assignValueNames
	 *
	 * @param n, Reference to a node
	 *
 	 * Using the ObservationsMapR_ this method generates a proper name
	 * vector for the values in this node. They are used as column names
	 * in the observation matrix and the CPT.
	 */
	void assignValueNames(Node& n);

	/**getObservationColIndex
	 *
	 * @param sample, value of interest
	 * @param n, A reference to a node
	 *
	 * @return The Index of the given value in the value list of the node
	 *
	 */
	int getObservationColIndex(unsigned int sample, const Node& n);

	/**getObservationRowIndex
	 *
	 * @param sample, 
	 * @param n, A reference to a node
	 * @param obsMatrix, A const reference to th observation matrix
	 *
	 * @return The index of the given value in the parent value list of the node
 	 *
	 */
	int getObservationRowIndex(unsigned int sample, const Node& n, const Matrix<int>& obsMatrix);

	/**assignParentNames
	 *
	 * @param n, A reference to the node in question
	 *
 	 * This method computes the original string representation for
	 * all parent value combinations. These are used row Names
	 * in the observation matrix and CPT, as they are more interpretable by the user
	 */
	void assignParentNames(Node& n);

	/**countObservations
	 *
	 * @param obsMatrix, A reference to a Matrix of typ int, containing the discretised observations
	 * @param n, A reference to a Node
	 *
 	 * This fills the observation matrix for a node, by iterating over the discretised samples
	 */
	void countObservations(Matrix<int>& obsMatrix, const Node& n);
	// A reference to the network
	Network& network_;	
	// A reference to the observation matrix
	Matrix<int>& observations_;
	// A map from the original value names to the internal integer representation
	std::unordered_map<std::string,int>& observationsMap_;
	// A map from the internal integer representation (using the observationRow entry in the Node class) to the original string representation
	std::map<std::pair<int,int>, std::string>& observationsMapR_;
};

#endif
