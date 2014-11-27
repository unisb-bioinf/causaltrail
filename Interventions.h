#ifndef INTERVENTIONS_H
#define INTERVENTIONS_H

#include "NetworkController.h"
class Interventions{
	public:
	Interventions();
	Interventions(NetworkController& c);
	void createBackupOfNetworkStructure();
	void loadBackupOfNetworkStructure();
	void doIntervention(const std::string& NodeName, const std::string& value);
	void doIntervention(int nodeID, int value);
	void reverseDoIntervention(const std::string& NodeName);
	void reverseDoIntervention(int nodeID);
	void addEdge(const std::string& source, const std::string& target);
	void addEdge(int source, int target);
	void removeEdge(const std::string& source, const std::string& target);
	void removeEdge(int source, int target);

	private:
	NetworkController& controller_;
};
#endif
