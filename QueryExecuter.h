#ifndef QUERYEXECUTER
#define QUERYEXECUTER
#include "ProbabilityHandler.h"
#include "Interventions.h"
#include "NetworkController.h"

class QueryExecuter{

	public:
	QueryExecuter(NetworkController& c);
	void setNonIntervention(const int nodeID, const int valueID);
	void setCondition(const int nodeID,const int valueID);
	void setDoIntervention(const int nodeID, const int valueID);
	void setAddEdge(const int nodeID1, const int nodeID2);
	void setRemoveEdge(const int nodeID1, const int nodeID2);
	void setArgMax(const int nodeID);

	private:
	ProbabilityHandler probHandler_;
	Interventions interventions_;
	NetworkController& networkController_;	
	std::vector<int> nonInterventionNodeID_;
	std::unordered_map<int, unsigned int> nonInterventionValues_;
	std::vector<int> conditionNodeID_;
	std::unordered_map<int, unsigned int> conditionValues_;
	std::vector<int> doInterventionNodeID_;
	std::unordered_map<int, unsigned int> doInterventionValues_;
	std::vector<std::pair<int, int>> addEdgeNodeIDs_;
	std::vector<std::pair<int, int>> removeEdgeNodeIDs_;
	std::vector<int> argmaxNodeIDs_;
	
};

#endif
