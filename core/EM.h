#ifndef EM_H
#define EM_H

#include "ProbabilityHandler.h"

#include <cmath>
#include <chrono>

class EM{
	public:
	/**
	 * This class performs the EM algorithm as described in Probablistic Graphical Models by
	 * Koller & Friedmann.
	 * The network is fitted given the data using 2 different initialisation techniques. According to
	 * the log-likelihood, the most probable parameters are chosen.
	 *
	 * @param network A reference to the network
	 * @param observations_ A matrix of type int containing the discretised sample data
	 * @param differenceThreshold_ The threshold for convergence of the EM algorithm (default is 0.0001)
	 * @param maxRuns_ The allowed number of iterations for the EM algorithm (default is 10000)
	 *
	 */
	EM(Network& network, Matrix<int>& observations_,float differenceThreshold_ = 0.0001f, unsigned int maxRuns_=10000);	

	EM& operator=(const EM&) = delete;
	EM& operator=(EM&&) = delete;

	/**
	 * This method executes the algorithm. 
	 */
	void performEM();

	/**
	 * @return The Log-Likelihood of the data given the current network parameters
	 */
	float calculateLikelihoodOfTheData();

	/**
	 * @return The number of executed EM-iterations
	 */
	int getNumberOfRuns();

	/**
	 * @return The final(final and pre-final iteration) parameter difference
	 */
	float getDifference();

	/**
	 * @return Time in Microseconds used to execute the EM algorithm
	 */
	int getTimeInMicroSeconds();

	private:
	/**
	 * @param n A cost reference to the Node in question
	 * @param col column of the CPT
	 * @param row row of the CPT
	 *
	 * @return The probability at the specified position of the CPT
	 * in the specified node
	 */
	float calculateProbabilityEM(Node& n, unsigned int col, unsigned int row);

	/**
	 * Calculates the expected values for the given row and node. The expected value is
	 * the expected number of occurrences of certain observations. This is needed in the ePhase
	 * of the EM algorithm
	 *
	 * @param row row of the CPT
	 * @param n A reference to a node
	 */
	void calculateExpectedValue(unsigned int row, Node& n);

	/**
	 * Executes the ePhase of the EM algorithm
	 */
	void ePhase();

	/**
	 * Performs maximum likelihood estimation for the given node
	 *
	 * @param row row of interest
	 * @param counter a counter for the calculated parameters
	 * @param difference a reference to the parameter difference
	 * @param n a reference to the node of interest
	 * @param obMatrix a const reference to the observation matrix
	 */
	void calculateMaximumLikelihood(unsigned int row, unsigned int& counter, float& difference, Node& n, const Matrix<int> & obMatrix);

	/**
	 * Executes the mPhase of the EM algorithm.
	 *
	 * @return Difference of the parameters between the current and the previous state
	 */
	float mPhase();

	/**
	 * Calls different initialisation methods.
	 */
	void initalise();

	/**
	 * Initialises all parameters assuming an equal distribution.
	 */
	void initaliseAssumingUniformDistribution();

	/**
	 * Initialises all parameters using MLE for the observed values.
	 */
	void initaliseAccordingToInitialDistribution();

	std::pair<float, unsigned int> runEMIterations_();
	unsigned int getMaxMethod_();

	//A reference to the network
	Network& network_;
	//The initialisation method
	unsigned int method_;
	//The discretised observations
	Matrix<int>& observations_;
	//An instance of the probabilityHandler
	ProbabilityHandler probHandler_;
	//The parameter difference
	float differenceThreshold_;
	//Fields dealing with run information
	unsigned int maxRuns_;
	int neededRuns_;	
	//The resulting parameter difference
	float finalDifference_;
	//c++11 time measuring
	std::chrono::time_point<std::chrono::system_clock> start;
	std::chrono::time_point<std::chrono::system_clock> end;
};

#endif
