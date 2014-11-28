#include "QueryExecuter.h"

QueryExecuter::QueryExecuter(NetworkController& c)
	:networkController_(c),probHandler_(ProbabilityHandler(c.getNetwork())), interventions_(c)
{
	unsigned int size = c.getNetwork().size();
	nonInterventionValues_.resize(size,-1);
	conditionValues_.resize(size,-1);
	doInterventionValues_.resize(size,-1);
	
}

bool QueryExecuter::hasInterventions(){
	return doInterventionNodeID_.size() != 0 || addEdgeNodeIDs_.size() != 0 || removeEdgeNodeIDs_.size() != 0;
}

std::pair<float,std::vector<std::string>> QueryExecuter::execute(){
	if (hasInterventions()){
		executeInterventions();	
	}	
	std::pair<float,std::vector<std::string>> probability = computeProbability();
	if (hasInterventions()){
		reverseInterventions();
	}
	return probability;
}


std::pair<float,std::vector<std::string>> QueryExecuter::computeProbability(){
	std::vector<std::string> temp;
	if (not argmaxNodeIDs_.empty()){
		return executeArgMax();
	}
	else if (not conditionNodeID_.empty()){
		return std::make_pair(executeCondition(), temp);
	}
	else {
		return std::make_pair(executeProbability(), temp);
	}
}

void QueryExecuter::executeInterventions(){
	interventions_.createBackupOfNetworkStructure();
	bool topologyChange = false;
	if (not addEdgeNodeIDs_.empty()){
		executeEdgeAdditions();
		topologyChange = true;
	}
	if (not removeEdgeNodeIDs_.empty()){
		executeEdgeDeletions();
		topologyChange = true;
	}
	if (topologyChange){
		networkController_.trainNetwork();	
	}
	if (not doInterventionValues_.empty()){
		executeDoInterventions();
	}	
}

void QueryExecuter::reverseInterventions(){
	interventions_.loadBackupOfNetworkStructure();
	if (not doInterventionValues_.empty()){
		executeReverseDoInterventions();
	}
	bool topologyChange = false;
	if (not addEdgeNodeIDs_.empty()){
		executeEdgeAdditionsReverse();
		topologyChange=true;
	}
	if (not removeEdgeNodeIDs_.empty()){
		executeEdgeDeletionsReverse();
		topologyChange=true;
	}
	if (topologyChange){
		networkController_.trainNetwork();
	}
}

void QueryExecuter::executeDoInterventions(){
	for (unsigned int index = 0; index<doInterventionValues_.size(); index++){
		interventions_.doIntervention(doInterventionNodeID_[index], doInterventionValues_[doInterventionNodeID_[index]]);
	}
}

void QueryExecuter::executeReverseDoInterventions(){
	for (unsigned int index = 0; index<doInterventionValues_.size();index++){
		interventions_.reverseDoIntervention(doInterventionNodeID_[index]);
	}
}

void QueryExecuter::executeEdgeAdditions(){
	for (auto& p : addEdgeNodeIDs_){
		interventions_.addEdge(p.first,p.second);
	}
}

void QueryExecuter::executeEdgeDeletions(){
	for (auto& p : removeEdgeNodeIDs_){
		interventions_.removeEdge(p.first,p.second);
	}
}

void QueryExecuter::executeEdgeAdditionsReverse(){
	for (auto& p : addEdgeNodeIDs_){
		interventions_.removeEdge(p.first,p.second);
	}
}

void QueryExecuter::executeEdgeDeletionsReverse(){
	for (auto& p : removeEdgeNodeIDs_){
		interventions_.addEdge(p.first,p.second);
	}
}


std::pair<float, std::vector<std::string>> QueryExecuter::executeArgMax(){
	return probHandler_.maxSearch(argmaxNodeIDs_);
}

float QueryExecuter::executeCondition(){
	std::vector<unsigned int> nominatorNodes = nonInterventionNodeID_;
	nominatorNodes.insert(nominatorNodes.end(), conditionNodeID_.begin(), conditionNodeID_.end());
	std::vector<int> nominatorValues = nonInterventionValues_;
	for (auto& id : conditionNodeID_){
		nominatorValues[id]=conditionValues_[id];
	}
	return probHandler_.computeConditionalProbability(nominatorNodes, conditionNodeID_, nominatorValues, conditionValues_);
}

float QueryExecuter::executeProbability(){
	if (nonInterventionNodeID_.size() == 1){
		return probHandler_.computeTotalProbabilityNormalized(nonInterventionNodeID_[0],nonInterventionValues_[(nonInterventionNodeID_[0])]);
	}
	else{
		return probHandler_.computeJointProbability(nonInterventionNodeID_,nonInterventionValues_);
	}
}
	
void QueryExecuter::setNonIntervention(const unsigned int nodeID, const unsigned int valueID)
{
	nonInterventionNodeID_.push_back(nodeID);
	nonInterventionValues_[nodeID]=valueID;
}

void QueryExecuter::setCondition(const unsigned int nodeID, const unsigned int valueID)
{
	conditionNodeID_.push_back(nodeID);
	conditionValues_[nodeID]=valueID;
}

void QueryExecuter::setDoIntervention(const unsigned int nodeID, const unsigned int valueID)
{
	doInterventionNodeID_.push_back(nodeID);
	doInterventionValues_[nodeID]=valueID;
}

void QueryExecuter::setAddEdge(const unsigned int nodeID1, const unsigned int nodeID2)
{
	addEdgeNodeIDs_.push_back(std::make_pair(nodeID1, nodeID2));
}

void QueryExecuter::setRemoveEdge(const unsigned int nodeID1, const unsigned int nodeID2)
{
	removeEdgeNodeIDs_.push_back(std::make_pair(nodeID1, nodeID2));	
}

void QueryExecuter::setArgMax(const unsigned int nodeID) 
{
	argmaxNodeIDs_.push_back(nodeID);
}

std::ostream& operator<<(std::ostream& os,const QueryExecuter& qe){
	os<<"NonIntervention: "<<qe.nonInterventionNodeID_.size()<<" "<<qe.nonInterventionValues_.size()<<std::endl;
	for (unsigned int index = 0; index < qe.nonInterventionNodeID_.size(); index++){
		os<<qe.nonInterventionNodeID_[index]<<" "<<qe.nonInterventionValues_[qe.nonInterventionNodeID_[index]]<<std::endl;
	}
	os<<"Condition: "<<qe.conditionNodeID_.size()<<" "<<qe.conditionValues_.size()<<std::endl;
	for (unsigned int index = 0; index < qe.conditionNodeID_.size(); index++){
		os<<qe.conditionNodeID_[index]<<" "<<qe.conditionValues_[(qe.conditionNodeID_[index])]<<std::endl;
	}
	os<<"ArgMax: "<<qe.argmaxNodeIDs_.size()<<std::endl;
	for (unsigned int index = 0; index < qe.argmaxNodeIDs_.size(); index++){
		os<<qe.argmaxNodeIDs_[index]<<std::endl;
	}
	os<<"DoIntervention: "<<qe.doInterventionNodeID_.size()<<" "<<qe.doInterventionValues_.size()<<std::endl;
	for (unsigned int index = 0; index < qe.doInterventionNodeID_.size(); index++){
		os<<qe.doInterventionNodeID_[index]<<" "<<qe.doInterventionValues_[(qe.doInterventionNodeID_[index])]<<std::endl;
	}
	os<<"AddEdge: "<<qe.addEdgeNodeIDs_.size()<<std::endl;
	for (unsigned int index = 0; index < qe.addEdgeNodeIDs_.size(); index++){
		os<<qe.addEdgeNodeIDs_[index].first<<" "<<qe.addEdgeNodeIDs_[index].second<<std::endl;
	}
	os<<"RemoveEdge: "<<qe.removeEdgeNodeIDs_.size()<<std::endl;
	for (unsigned int index = 0; index < qe.removeEdgeNodeIDs_.size(); index++){
		os<<qe.removeEdgeNodeIDs_[index].first<<" "<<qe.removeEdgeNodeIDs_[index].second<<std::endl;
	}

return os;
}

