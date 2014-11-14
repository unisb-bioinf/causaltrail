#ifndef INTERVENTIONS_H
#define INTERVENTIONS_H

#include "NetworkController.h"
class Interventions{
	public:
	Interventions();
	Interventions(NetworkController& c);
	void createBackupOfNetworkStructure();
	void loadBackupOfNetworkStructure();
	void doIntervention(std::string NodeName, std::string value);
	void doIntervention(int nodeID, int value);
	void reverseDoIntervention(std::string NodeName, std::string value);
	void reverseDoIntervention(int nodeID, int value);
	void addEdge(std::string source, std::string target);
	void addEdge(int source, int target);
	void removeEdge(std::string source, std::string target);
	void removeEdge(int source, int target);

	private:
	NetworkController& controller_;
};
#endif
