#include "QueryExecuter.h"

QueryExecuter::QueryExecuter(NetworkController& c)
	:networkController_(c),probHandler_(ProbabilityHandler(networkController_.getNetwork())), interventions_(networkController_)
{
	
}
void QueryExecuter::setNonIntervention(const int nodeID, const int valueID)
{
	nonInterventionNodeID_.push_back(nodeID);
	nonInterventionValues_[nodeID]=valueID;
}

void QueryExecuter::setCondition(const int nodeID, const int valueID)
{
	conditionNodeID_.push_back(nodeID);
	conditionValues_[nodeID]=valueID;
}

void QueryExecuter::setDoIntervention(const int nodeID, const int valueID)
{
	doInterventionNodeID_.push_back(nodeID);
	doInterventionValues_[nodeID]=valueID;
}

void QueryExecuter::setAddEdge(const int nodeID1, const int nodeID2)
{
	addEdgeNodeIDs_.push_back(std::make_pair(nodeID1, nodeID2));
}

void QueryExecuter::setRemoveEdge(const int nodeID1, const int nodeID2)
{
	removeEdgeNodeIDs_.push_back(std::make_pair(nodeID1, nodeID2));	
}

void QueryExecuter::setArgMax(const int nodeID) 
{
	argmaxNodeIDs_.push_back(nodeID);
}

