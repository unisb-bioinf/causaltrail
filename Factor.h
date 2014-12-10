#ifndef FACTOR_H
#define FACTOR_H

#include "Network.h"

class Factor{
	public:
	/**Factor
	 *
	 * @param n, a const reference to a node
	 * @param values, a const reference to known values of the nodes
	 * 
	 * @return a Factor object
	 *
	 */
	Factor(const Node& n, const std::vector<int>& values);

	/**Factor
	 *
	 * @param length, number of different value combinations represented by the factor
	 * @param ids, vector of node identifiers represented by the factor
	 * 
	 * @return a Factor object
	 *
	 */
	Factor(unsigned int length, std::vector<unsigned int> ids);

	/**getIDs
	 *
	 * @return vector of node identifiers represented by the factor
	 *
	 */
	const std::vector<unsigned int>& getIDs() const ;

	/**addProbability
	 *
	 * @param
	 * @param
	 * 
	 * @return
	 *
	 *
	 */
	void addProbability(float prob);

	/**setProbability
	 *
	 * @param prob, probability to set
	 * @param index, position in the factor
	 * 
	 * Stores the given probability at the given position in the factor
	 */
	void setProbability(float prob, unsigned int index);

	/**getProbability
	 *
	 * @param index, position in the factor
	 * 
	 * @return Probability at the given position
	 *
	 */
	float getProbability(unsigned int index) const;

	/**getProbability
	 *
	 * @param
	 * @param
	 * 
	 * @return
	 *
	 *
	 */
	float getProbability(const std::vector<int>& values) const;
	
	/**getIndex
	 *
	 * @param index, identifier of the node of interest
	 * 
	 * @return position of the node specified with index in the node identifier list
	 *
	 */
	unsigned int getIndex(unsigned int index) const;

	/**product
	 *
	 * @param factor, a reference to the Factor to form the product with
	 * @param network_, a const reference to the network
	 * @param values, a const reference to a vector containing known values for the nodes
	 * 
	 * @return a new Factor representing the product of the former two
	 *
	 * Performs a product operation on two factors
	 */
	Factor product(Factor& factor,const Network& network_, const std::vector<int>& values);

	/**sumOut
	 *
	 * @param id, identifier of the node to be summed out
	 * @param network_, a reference to the network
	 * @param values, a reference to a vector containing known values for the nodes
	 *
	 * @return a new Factor representing the result of summing out the node with the given id
	 *
	 */
	Factor sumOut(unsigned int id, Network& network_, const std::vector<int>& values);

	/**normalize
	 *
	 * Normalizes the entries of a factor, such that they sum up to one.
	 */
	void normalize();

	/**operator<<
	 *
	 * @param os, ostream reference
	 * @param f, const reference to a Factor
	 * 
	 * @return ostream
	 *
	 * Ostream operator implementation for a factor
	 */
	friend std::ostream& operator<< (std::ostream& os,const Factor& f);
	
	private:

	/**getUnionOfIDs
	 *
	 * @param commonIDs, common ids between this factor and the factor to form the union with
	 * @param factor, a reference to the factor to join ids with
	 * 
	 * @return vector containing the union of IDs
	 *
	 */
	std::vector<unsigned int> getUnionOfIDs(const std::vector<unsigned int>& commonIDs, const Factor& factor) const;

	/**getCommonIDs
	 *
	 * @param factor, Factor to find common IDs 
	 * 
	 * @return vector containing common ids between this and the given factor
	 *
	 */
	std::vector<unsigned int> getCommonIDs(const Factor& factor) const;

	//Vector of node identifieres contained in this node
	std::vector<unsigned int> nodeIDs_;

	//Vector representing the values of the represented nodes
	std::vector<int> val_;

	//vector containing the probabilities of the factor
	std::vector<float> probabilities_;

	//Number of different value combinations contained in the factor
	int length_;
};

#endif
