#ifndef EM_H
#define EM_H

#include "ProbabilityHandler.h"

#include <cmath>
#include <chrono>

class EM{
	public:
	/**EM
	 *
	 * @param network, A reference to the network
 	 * @param observations_, A matrix of typ int containing the discretised sample data
	 * @param differenceThreshold_, The threshold for convergence of the EM algorithm (default is 0.0001)
	 * @param maxRuns_, The allowed number of iterations for the EM algorithm (default is 10000)
 	 *
	 * @return an object of class EM
	 *
 	 * This class performs the EM algorithm as described in Probablistic Graphical Models by
	 * Koller & Friedmann. 
	 * The network is fitted given the data using 2 different initilisation technqiues. According to 
	 * the log-likelyhood, the most probable parameters are chosen.
	 */
	EM(Network& network, Matrix<int>& observations_,float differenceThreshold_ = 0.0001f, unsigned int maxRuns_=10000);	

	/**EM
	 *
	 * This method executes the algorithm. 
	 */
	void performEM();
	/**EM
	 *
	 * @return The Log-Likelihood of the data given the current network parameters
	 *
	 */
	float calculateLikelihoodOfTheData();

	/**EM
	 *
	 * @return The number of executed EM-iterations
 	 *
	 */
	int getNumberOfRuns();

	/**EM
	 *
	 * @return The final(final and prefinal iteration) parameter difference
	 *
	 */
	float getDifference();

	/**EM
	 *
	 * @return Time in Microseconds used to execute the EM algorithm
 	 *
	 */
	int getTimeInMicroSeconds();
	private:

	/**EM
	 *
	 * @param n, A cost reference to the Node in question
 	 * @param col, column of the CPT
	 * @param row, row of the CPT
 	 *
	 * @return The probability at the specified position of the CPT
	 * in the specified node
 	 *
	 */
	float calculateProbabilityEM(Node& n, unsigned int col, unsigned int row);

	/**EM
	 *
	 * @param row, row of the CPT
 	 * @param n, A reference to a node
 	 *
 	 * Calculates the expected values for the given row and node. The expected value is
	 * the expected number of occurences of certain observations. This is neeed in the ePhase
	 * of the EM algorithm
	 */
	void calculateExpectedValue(unsigned int row, Node& n);

	/**EM
	 *
 	 *Executes the ePhase of the EM algorithm
	 */
	void ePhase();

	/**EM
	 *
	 * @param row, row of interest
 	 * @param counter, a counter for the calculated parameters
	 * @param difference, a reference to the parameter difference
	 * @param n, a reference to the node of interest
	 * @param obMatrix, a const reference to the observation matrix
 	 * 
 	 * Performs maximum likelihood estimation for the given node
	 */
	void calculateMaximumLikelihood(unsigned int row, unsigned int& counter, float& difference, Node& n, const Matrix<int> & obMatrix);

	/**EM
	 *
	 * @return Difference of the parameters between the current and the previous state
	 *
 	 * Executes the mPhase of the EM algorithm
	 */
	float mPhase();

	/**EM
	 *
	 * Calls different initialisation methods
	 */
	void initalise();

	/**EM
	 *
 	 * Initialises all parameters assuming an equal distribution
	 */
	void initalise1();

	/**EM
	 *
	 *
 	 * Initialises all parameters using MLE for the observed values
	 */
	void initalise2();

	//A reference to the network
	Network& network_;
	//The initilisation method
	unsigned int method_;
	//The discretised observations
	Matrix<int>& observations_;
	//An instance of the probabilitiyHandler
	ProbabilityHandler probHandler_;
	//The parameter difference
	float differenceThreshold_;
	//Fields dealing with run information
	unsigned int maxRuns_;
	int neededRuns_;	
	//The resulting parameter differenec
	float finalDifference_;
	//c++11 time measuring
	std::chrono::time_point<std::chrono::system_clock> start;
	std::chrono::time_point<std::chrono::system_clock> end;
};

#endif
