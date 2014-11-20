#ifndef QUERYEXECUTER
#define QUERYEXECUTER
#include "ProbabilityHandler.h"
#include "Interventions.h"
#include "NetworkController.h"

class QueryExecuter{

	public:
	QueryExecuter(NetworkController& c);
	std::pair<float,std::vector<std::string>> execute();
	void setNonIntervention(const unsigned int nodeID, const unsigned int valueID);
	void setCondition(const unsigned int nodeID,const unsigned int valueID);
	void setDoIntervention(const unsigned int nodeID, const unsigned int valueID);
	void setAddEdge(const unsigned int nodeID1, const unsigned int nodeID2);
	void setRemoveEdge(const unsigned int nodeID1, const unsigned int nodeID2);
	void setArgMax(const unsigned int nodeID);
	friend std::ostream& operator<< (std::ostream& os, const QueryExecuter& qe);

	private:
	bool hasInterventions();
	void executeInterventions();
	void reverseInterventions();
	std::pair<float,std::vector<std::string>> computeProbability();
	void executeDoInterventions();
	void executeReverseDoInterventions();
	void executeEdgeAdditions();
	void executeEdgeDeletions();
	void executeEdgeAdditionsReverse();
	void executeEdgeDeletionsReverse();	
	std::pair<float,std::vector<std::string>> executeArgMax();
	float executeCondition();
	float executeProbability();
	ProbabilityHandler probHandler_;
	Interventions interventions_;
	NetworkController& networkController_;	
	std::vector<unsigned int> nonInterventionNodeID_;
	std::unordered_map<unsigned int, int> nonInterventionValues_;
	std::vector<unsigned int> conditionNodeID_;
	std::unordered_map<unsigned int, int> conditionValues_;
	std::vector<unsigned int> doInterventionNodeID_;
	std::unordered_map<unsigned int, int> doInterventionValues_;
	std::vector<std::pair<unsigned int, unsigned int>> addEdgeNodeIDs_;
	std::vector<std::pair<unsigned int, unsigned int>> removeEdgeNodeIDs_;
	std::vector<unsigned int> argmaxNodeIDs_;
};

#endif
