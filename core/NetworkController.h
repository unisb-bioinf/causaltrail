#ifndef NETWORKCONTROLLER_H
#define NETWORKCONTROLLER_H

#include "Matrix.h"
#include "Network.h"

#include <string>
#include <vector>

class Discretiser;
class DiscretisationSettings;

/**
 * This class handles reading of the network structure
 * and parameter learning.
 */
class NetworkController
{
	public:

	/**
	 * Default constructor for NetworkController
	 */
	NetworkController();

	/**
	 * Generates a network representation based on the networkfile. Uses the
	 * readNetwork method in the Network class.
	 *
	 * @param networkfile file containing network structure information
	 */
	void loadNetwork(const std::string& networkfile);

	/**
	 * Reads the raw sample data and performs the discretisation
	 *
	 * @param d The discretiser that should be used.
	 */
	void loadObservations(Discretiser& d);


	/**
	 * Reads the raw sample data and performs the discretisation
	 *
	 * @param datafile file containing the raw sample data
	 * @param controlFile File containing the discretiser parameters
	 */
	void loadObservations(const std::string& datafile, const std::string& controlFile);

	/**
	 * Reads the raw sample data and performs the discretisation
	 *
	 * @param datafile file containing the raw sample data
	 * @param controlFile controlFile for discretisation
	 * @param samplesToDelete vector containing the column index of samples that should not be read
	 */
	void loadObservations(const std::string& datafile, const std::string& controlFile, const std::vector<unsigned int>& samplesToDelete);

	/**
	 * Reads the raw sample data and performs the discretisation.
	 *
	 * @param datafile File containing the raw sample data.
	 * @param settings Parameters used for discretisation.
	 */
	void loadObservations(const std::string& datafile, const DiscretisationSettings& settings);

	/**
	 * Reads the raw sample data and performs the discretisation.
	 *
	 * @param datafile File containing the raw sample data.
	 * @param settings Parameters used for discretisation.
	 * @param samplesToDelete Vector containing the column index of samples that should not be read.
	 */
	void loadObservations(const std::string& datafile, const DiscretisationSettings& settings, const std::vector<unsigned int>& samplesToDelete);

	/**
	 * Trains the network using the EM algorithm
	 */
	void trainNetwork();

	/**
	 * @return the log-likelihood of the data
	 */
	float getLikelihoodOfTheData() const;

	/**
	 * @return a reference to the network
	 */
	Network& getNetwork();

	/**
	 * @return a reference to the network
	 */
	const Network& getNetwork() const;

	/**
	 * @return the number of executed EM iterations
	 */
	int getNumberOfEMRuns() const;

	/**
	 * @return the final Parameter difference in EM
	 */ 
	float getParameterDifference() const;

	/**
	 * @return time in microseconds used to perform EM
	 */
	int getTimeInMicroSeconds() const;

	/**
	 * Stores all network parameters in a file.
	 * The filename incorporates the data and time of its generation
	 */
	void saveParameters() const;

	/**
	 * Checks whether an edge can be added to a network without inducing a cycle.
	 *
	 * @param sourceID Identifier of the source node.
	 * @param targetID Identifier of the target node.
	 * @param addedEdges Edges that have been added as part of a query.
	 * @param removedEdges Edges that have been removed as part of a query.
	 *
	 * @return true, if the edge does not induce a cycle, false otherwise
	 */
	bool isEdgePossible(unsigned int sourceID, unsigned int targetID,
	                    std::vector<std::pair<unsigned int, unsigned int>>& addedEdges,
	                    std::vector<std::pair<unsigned int, unsigned int>>& removedEdges);

	/**storeDiscretisedData
	 *
	 * @param filename Name of the file to write the discretised data to
	 */
	void storeDiscretisedData(const std::string& filename) const;	
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
