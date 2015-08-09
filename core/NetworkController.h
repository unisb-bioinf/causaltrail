#ifndef NETWORKCONTROLLER_H
#define NETWORKCONTROLLER_H

#include "Matrix.h"
#include "Network.h"

#include <string>
#include <vector>

class Discretiser;
class DiscretisationSettings;

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
	 * @param datafile file containing the raw sample data
	 * @param control specification how the input data should be discretised
	 *
	 * Reads the raw sample data and performs the discretisation
	 */
	void loadObservations(Discretiser& d);


	/**loadObservations
	 *
	 * @param datafile, file containing the raw sample data
	 * @param controlFile, controlFile for discretisation
	 *
	 * Reads the raw sample data and performs the discretisation
	 */
	void loadObservations(const std::string& datafile, const std::string& controlFile);

	/**loadObservations
	*
	* @param datafile, file containing the raw sample data
	* @param controlFile, controlFile for discretisation
	* @param samplesToDelete, vector containing the column index of samples that should not be read
	*
	* Reads the raw sample data and performs the discretisation
	*/
	void loadObservations(const std::string& datafile, const std::string& controlFile, const std::vector<unsigned int>& samplesToDelete);

	/**loadObservations
	*
	* @param datafile, file containing the raw sample data
	* @param controlFile, controlFile for discretisation
	*
	* Reads the raw sample data and performs the discretisation
	*/
	void loadObservations(const std::string& datafile, const DiscretisationSettings& propertyTree);

	/**loadObservations
	*
	* @param datafile, file containing the raw sample data
	* @param controlFile, controlFile for discretisation
	* @param samplesToDelete, vector containing the column index of samples that should not be read
	*
	* Reads the raw sample data and performs the discretisation
	 */
	void loadObservations(const std::string& datafile, const DiscretisationSettings& propertyTree, const std::vector<unsigned int>& samplesToDelete);

	/**trainNetwork
	 *
	 * Trains the network using the EM algorithm
	 */
	void trainNetwork();

	/**getLikelihoodOfTheData
	 *
	 * @return the Log Likelyhood of the data
	 */
	float getLikelihoodOfTheData() const;

	/**getNetwork
	 *
	 * @return a reference to the network
	 */	 
	Network& getNetwork();

	/**getNetwork
	 *
	 * @return a reference to the network
	 */
	const Network& getNetwork() const;

	/**getNumberOfEMRuns
	 *
	 * @return the number of executed EM iterations
	 */
	int getNumberOfEMRuns() const;

	/**getParameterDifference
	 *
	 * @return the final Parameter difference in EM
	 */ 
	float getParameterDifference() const;

	/**getTimeInMicroSeconds
	 *
	 * @return time in microseconds used to perform EM
	 */
	int getTimeInMicroSeconds() const;

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
	bool isEdgePossible(unsigned int sourceID, unsigned int targetID, std::vector<std::pair<unsigned int, unsigned int>>& addedEdges, std::vector<std::pair<unsigned int, unsigned int>>& removedEdges);


	/**storeDiscretisedData
	 *
	 * @param filename, Name of the file to write the discretised data to
	 */
	void storeDiscretisedData(const std::string& filename);	
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
