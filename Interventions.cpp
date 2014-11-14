#include "Interventions.h"

/*Interventions
 *
 * @param
 *
 * @return
 *
 * Description
 */	
Interventions::Interventions(NetworkController& controller)
	:controller_(controller){

}

/*createBackupOfNetworkStructure
 *
 * @param
 *
 * @return
 *
 * Description
 */	
void Interventions::createBackupOfNetworkStructure(){
	Network& network = controller_.getNetwork();
	network.createBackup();
}

void Interventions::loadBackupOfNetworkStructure(){
	Network& network = controller_.getNetwork();
	network.loadBackup();
}

/*doIntervention
 *
 * @param
 *
 * @return
 *
 * Description
 */	
void Interventions::doIntervention(std::string NodeName, std::string value){
	Network& network = controller_.getNetwork();
	Node& n = network.getNode(NodeName);
	n.createBackupDoIntervention();
	network.cutParents(NodeName);
	n.setProbabilityTo1(value);
}

/*doIntervention
 *
 * @param
 *
 * @return
 *
 * Description
 */	
void Interventions::doIntervention(int nodeID, int value){
	Network& network = controller_.getNetwork();
	Node& n = network.getNode(nodeID);
	n.createBackupDoIntervention();
	network.cutParents(nodeID);
	n.setProbabilityTo1(value);
}
/*reverseDoIntervention
 *
 * @param
 *
 * @return
 *
 * Description
 */	
void Interventions::reverseDoIntervention(std::string NodeName, std::string value){
	Network& network = controller_.getNetwork();
	Node& n = network.getNode(NodeName);
	n.loadBackupDoIntervention();
}

/*reverseDoIntervention
 *
 * @param
 *
 * @return
 *
 * Description
 */	
void Interventions::reverseDoIntervention(int nodeID, int value){
	Network& network = controller_.getNetwork();
	Node& n = network.getNode(nodeID);
	n.loadBackupDoIntervention();
}

/*addEdge
 *
 * @param
 *
 * @return
 *
 * Description
 */	
void Interventions::addEdge(std::string source, std::string target){
	Network& network = controller_.getNetwork();
	network.addEdge(target,source);
}

/*addEdge
 *
 * @param
 *
 * @return
 *
 * Description
 */	
void Interventions::addEdge(int source, int target){
	Network& network = controller_.getNetwork();
	network.addEdge(target,source);
}

/*removeEdge
 *
 * @param
 *
 * @return
 *
 * Description
 */	
void Interventions::removeEdge(std::string source, std::string target){
	Network& network = controller_.getNetwork();
	network.removeEdge(target,source);
}

/*removeEdge
 *
 * @param
 *
 * @return
 *
 * Description
 */	
void Interventions::removeEdge(int source, int target){
	Network& network = controller_.getNetwork();
	network.removeEdge(target,source);
}
