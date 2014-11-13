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
	network.loadBackup();
	Node& n = network.getNode(NodeName);
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
	network.addEdge(source,target);
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
	network.removeEdge(source,target);
}


