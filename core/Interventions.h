#ifndef INTERVENTIONS_H
#define INTERVENTIONS_H

#include "NetworkController.h"
class Interventions{
	public:

	/**Interventions
	 *
 	 * @return Interventions Object
	 *
	 * This class allows the performance of Interventions in the network
	 * Supported are: Do-Interventions, Addition of Edges, Deletion of Edges
	 * Note that the addition or deletion of edges requires the network to be 
	 * retrained. Due to efficiency reasons, training is not called in this class.
	 * Retraining the network is called in the QueryExecuter class.
	 */
	Interventions();

	/**Interventions
	 *
	 * @param c, NetworkController
	 * 
	 * @return Interventions Object
	 *
	 * This class allows the performance of Interventions in the network
	 * Supported are: Do-Interventions, Addition of Edges, Deletion of Edges
 	 * Note that the addition or deletion of edges requires the network to be 
	 * retrained. Due to efficiency reasons, training is not called in this class.
	 * Retraining the network is called in the QueryExecuter class.
	 */
	Interventions(NetworkController& c);

	Interventions& operator=(const Interventions&) = delete;
	Interventions& operator=(Interventions&&) = delete;

	/**createBackupOfNetworkStructure
	 *
	 * Creates a backup of the network structure to be able
	 * to restore the original structure after the performance of interventions
	 */
	void createBackupOfNetworkStructure();

	/**loadBackupOfNetworkStructure
	 *
	 * Loads the backup of the network structure
	 */
	void loadBackupOfNetworkStructure();

	/**doIntervention
	 *
	 * @param NodeName, name of the node for whom the Do-Intervention should be performed
	 * @param value, value for which the probability should be set to 1.0
	 *
	 * Performs a Do Intervention of at the given node and the given value
	 */
	void doIntervention(const std::string& NodeName, const std::string& value);

	/**doIntervention
	 *
	 * @param nodeID, identifier of the node for whom the Do-Intervention should be performed
	 * @param value, value for which the probability should be set to 1.0
	 *
	 * Performs a Do Intervention of at the given node and the given value *
	 */
	void doIntervention(int nodeID, int value);

	/**reverseDoIntervention
	 *
	 * @param NodeName, name of the node for whom the Do-Intervention should be reversed
	 *
	 * Reverses a previous Do-Intervention at the given node
	 */
	void reverseDoIntervention(const std::string& NodeName);

	/**reverseDoIntervention
	 *
	 * @param nodeID, identifier of the node for whom the Do-Intervention should be reversed
	 *
	 * Reverses a previous Do-Intervention at the given node
	 */
	void reverseDoIntervention(int nodeID);

	/**addEdge
	 *
	 * @param source, name of the source node 
 	 * @param target, name of the target node
	 *
	 * Adds an edge from the source to the target node
	 */
	void addEdge(const std::string& source, const std::string& target);

	/**addEdge
	 *
	 * @param source, identifier of the source node
	 * @param target, identifier of the target node
	 * 
	 * Adds an edge from the source to the target node
	 */
	void addEdge(int source, int target);

	/**removeEdge
	 *
	 * @param source, identifier of the source node
	 * @param target, identifier of the target node
	 *
 	 * Removes the edge from the source to the target node
	 */
	void removeEdge(const std::string& source, const std::string& target);

	/**removeEdge
	 *
	 * @param source, identifier of the source node 
	 * @param target, identifier of the target node
	 *
 	 * Removes the edge from the the source to the target
	 */
	void removeEdge(int source, int target);

	private:

	//A reference to the NetworkController 
	NetworkController& controller_;

};
#endif
