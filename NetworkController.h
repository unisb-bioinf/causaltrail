#ifndef NETWORKCONTROLLER_H
#define NETWORKCONTROLLER_H
#include "Network.h"
#include "EM.h"
#include "Discretiser.h"
#include "DataDistribution.h"
#include <cmath>
#include <algorithm>
#include <utility>
#include <map>

class NetworkController{
	public:

	/**NetworkController
	 *
	 * Default constructor Network Controller
	 * This class handles reading of the network structure
	 * and parameter learning.
	 */
	NetworkController();

	/**loadNetwork
	 *
 	 * @param networkfile, file containing network structure information
	 * 
	 * Generates a network representation based on the networkfile. Uses the
	 * readNetwork method in the network Class
	 */
	void loadNetwork(const std::string& networkfile);

	/**loadObservations
	 *
	 * @param datafile, file containing the raw sample data
	 * @param controlFile, controlFile for discretisation
	 *
	 * Reads the raw sample data and performs the discretisation
	 */
	void loadObservations(const std::string& datafile, const std::string& controlFile);

	/**trainNetwork
	 *
	 * Trains the network using the EM algorithm
	 */
	void trainNetwork();

	/**getLikelihoodOfTheData
	 *
	 * @return the Log Likelyhood of the data
	 */
	const float& getLikelihoodOfTheData() const;

	/**getNetwork
	 *
	 * @return a reference to the network
	 */	 
	Network& getNetwork();
	
	/**getNumberOfEMRuns
	 *
	 * @return the number of executed EM iterations
	 */
	const int& getNumberOfEMRuns() const;

	/**getParameterDifference
	 *
	 * @return the final Parameter difference in EM
	 */ 
	const float& getParameterDifference() const;

	/**getTimeInMicroSeconds
	 *
	 * @return time in microseconds used to perform EM
	 */
	const int& getTimeInMicroSeconds() const;

	/**saveParameters
	 *
	 * Stores all network parameters in a file.
	 * The filename incoorpartes the data and time of its generation
	 */
	void saveParameters() const;

	/**isEdgePossible
	 *
	 * @param sourceID, Identifier of the source node
	 * @param targetID, Identifier of the target node
	 * @return true, if the edge does not induce a cycle, false otherwise
	 */
	bool isEdgePossible(unsigned int sourceID, unsigned int targetID);
	
	private:

	//Network object
	Network network_;

	//Matrix containing the discretised observations
	Matrix<int> observations_;

	//Number of EM runs
	int eMRuns_;

	//Final EM parameter difference
	float finalDifference_;

	//Log Likelihood of the data
	float likelihoodOfTheData_;

	//Time in microseconds to perform EM
	int timeInMicroSeconds_;
};
#endif
