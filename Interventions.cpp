#include "Interventions.h"

Interventions::Interventions(NetworkController& controller)
	:controller_(controller){

}

void Interventions::createBackupOfNetworkStructure(){
	Network& network = controller_.getNetwork();
	network.createBackup();
}

void Interventions::loadBackupOfNetworkStructure(){
	Network& network = controller_.getNetwork();
	network.loadBackup();
}

void Interventions::doIntervention(const std::string& NodeName, const std::string& value){
	Network& network = controller_.getNetwork();
	Node& n = network.getNode(NodeName);
	n.createBackupDoIntervention();
	network.cutParents(NodeName);
	std::cout<<"Before"<<std::endl;
	n.setProbabilityTo1(value);
	std::cout<<"After"<<std::endl;
}

void Interventions::doIntervention(int nodeID, int value){
	Network& network = controller_.getNetwork();
	Node& n = network.getNode(nodeID);
	n.createBackupDoIntervention();
	network.cutParents(nodeID);
	n.setProbabilityTo1(value);
}

void Interventions::reverseDoIntervention(const std::string& NodeName){
	Network& network = controller_.getNetwork();
	Node& n = network.getNode(NodeName);
	n.loadBackupDoIntervention();
}

void Interventions::reverseDoIntervention(int nodeID){
	Network& network = controller_.getNetwork();
	Node& n = network.getNode(nodeID);
	n.loadBackupDoIntervention();
}

void Interventions::addEdge(const std::string& source, const std::string& target){
	Network& network = controller_.getNetwork();
	network.addEdge(target,source);
}

void Interventions::addEdge(int source, int target){
	Network& network = controller_.getNetwork();
	network.addEdge(target,source);
}

void Interventions::removeEdge(const std::string& source, const std::string& target){
	Network& network = controller_.getNetwork();
	network.removeEdge(target,source);
}

void Interventions::removeEdge(int source, int target){
	Network& network = controller_.getNetwork();
	network.removeEdge(target,source);
}
