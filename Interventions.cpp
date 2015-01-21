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
	std::cout<<"Entering Intervention"<<std::endl;
	Network& network = controller_.getNetwork();
	std::cout<<"1"<<std::endl;
	Node& n = network.getNode(nodeID);
	std::cout<<"2"<<std::endl;
	n.createBackupDoIntervention();
	std::cout<<"3"<<std::endl;
	network.cutParents(nodeID);
	std::cout<<"Before"<<std::endl;
	n.setProbabilityTo1(value);
	std::cout<<"After"<<std::endl;n.setProbabilityTo1(value);
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
