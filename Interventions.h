#ifndef INTERVENTIONS_H
#define INTERVENTIONS_H

#include "NetworkController.h"
class Interventions{
	public:
	Interventions();
	Interventions(NetworkController& c);
	void createBackupOfNetworkStructure();
	void doIntervention(std::string NodeName, std::string value);
	void reverseDoIntervention(std::string NodeName, std::string value);
	void addEdge(std::string source, std::string target);
	void removeEdge(std::string source, std::string target);

	private:
	NetworkController& controller_;
};
#endif
