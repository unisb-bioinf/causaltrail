#ifndef INTERVENTIONS_H
#define INTERVENTIONS_H

#include "NetworkController.h"
/*
 * This class allows the performance of Interventions in the network
 * Supported are: Do-Interventions, Addition of Edges, Deletion of Edges
 * Note that the addition or deletion of edges requires the network to be
 * retrained. Due to efficiency reasons, training is not called in this class.
 * Retraining the network is called in the QueryExecuter class.
 */
class Interventions{
	public:

	/**
	 * Default constructor
	 */
	Interventions();

	/**
	 * Detailed constructor
	 *
	 * @param c NetworkController
	 */
	explicit Interventions(NetworkController& c);

	Interventions(const Interventions& o)
		: controller_(o.controller_)
	{
	}

	Interventions& operator=(const Interventions&) = delete;
	Interventions& operator=(Interventions&&) = delete;

	/**
	 * Creates a backup of the network structure to be able
	 * to restore the original structure after the performance of interventions
	 */
	void createBackupOfNetworkStructure();

	/**
	 * Loads the backup of the network structure
	 */
	void loadBackupOfNetworkStructure();

	/**
	 * Performs a Do Intervention of at the given node and the given value.
	 *
	 * @param NodeName Name of the node for whom the Do-Intervention should be performed.
	 * @param value Value for which the probability should be set to 1.0.
	 */
	void doIntervention(const std::string& NodeName, const std::string& value);

	/**
	 * Performs a Do Intervention of at the given node and the given value
	 *
	 * @param nodeID identifier of the node for whom the Do-Intervention should be performed
	 * @param value value for which the probability should be set to 1.0
	 */
	void doIntervention(int nodeID, int value);

	/**
	 * Reverses a previous Do-Intervention at the given node
	 *
	 * @param NodeName name of the node for whom the Do-Intervention should be reversed
	 */
	void reverseDoIntervention(const std::string& NodeName);

	/**
	 * Reverses a previous Do-Intervention at the given node.
	 *
	 * @param nodeID identifier of the node for whom the Do-Intervention should be reversed,
	 */
	void reverseDoIntervention(int nodeID);

	/**
	 * Adds an edge from the source to the target node
	 *
	 * @param source name of the source node
	 * @param target name of the target node
	 */
	void addEdge(const std::string& source, const std::string& target);

	/**
	 * Adds an edge from the source to the target node
	 *
	 * @param source identifier of the source node
	 * @param target identifier of the target node
	 */
	void addEdge(int source, int target);

	/**
	 * Removes the edge from the source to the target node
	 *
	 * @param source identifier of the source node
	 * @param target identifier of the target node
	 */
	void removeEdge(const std::string& source, const std::string& target);

	/**
	 * Removes the edge from the the source to the target
	 *
	 * @param source identifier of the source node
	 * @param target identifier of the target node
	 */
	void removeEdge(int source, int target);

	private:

	/// A reference to the NetworkController
	NetworkController& controller_;

};

#endif
