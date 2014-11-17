#include "QueryExecuter.h"

QueryExecuter::QueryExecuter(NetworkController& c)
	:networkController_(c),probHandler_(ProbabilityHandler(c.getNetwork())), interventions_(c)
{
	
}

bool QueryExecuter::hasInterventions(){
	return doInterventionNodeID_.size() != 0 || addEdgeNodeIDs_.size() != 0 || removeEdgeNodeIDs_.size() != 0;
}

void QueryExecuter::execute(){
	if (hasInterventions()){
		executeInterventions();	
	}	
	computeProbability();
	if (hasInterventions()){
		reverseInterventions();
	}
}


void QueryExecuter::computeProbability(){
	if (not argmaxNodeIDs_.empty()){
		std::cout<<"Searching maximum"<<std::endl;
		executeArgMax();
	}
	else if (not conditionNodeID_.empty()){
		std::cout<<"Calculating conditional probability"<<std::endl;
		executeCondition();
	}
	else {
		std::cout<<"Calculating query probability"<<std::endl;
		executeProbability();
	}
}

void QueryExecuter::executeInterventions(){
	std::cout<<"Interventions found!"<<std::endl;
	interventions_.createBackupOfNetworkStructure();
	bool topologyChange = false;
	if (not addEdgeNodeIDs_.empty()){
		std::cout<<"Adding edges"<<std::endl;
		executeEdgeAdditions();
		topologyChange = true;
	}
	if (not removeEdgeNodeIDs_.empty()){
		std::cout<<"Deleting edges"<<std::endl;
		executeEdgeDeletions();
		topologyChange = true;
	}
	if (topologyChange){
		networkController_.trainNetwork();	
	}
	if (not doInterventionValues_.empty()){
		std::cout<<"Performing do Intervention"<<std::endl;
		executeDoInterventions();
	}	
}

void QueryExecuter::reverseInterventions(){
	interventions_.loadBackupOfNetworkStructure();
	if (not doInterventionValues_.empty()){
		std::cout<<"Reversing do Intervention"<<std::endl;
		executeReverseDoInterventions();
	}
	bool topologyChange = false;
	if (not addEdgeNodeIDs_.empty()){
		std::cout<<"Removing added edges"<<std::endl;
		executeEdgeAdditionsReverse();
		topologyChange=true;
	}
	if (not removeEdgeNodeIDs_.empty()){
		std::cout<<"Adding deleted edges"<<std::endl;
		executeEdgeDeletionsReverse();
		topologyChange=true;
	}
	if (topologyChange){
		networkController_.trainNetwork();
	}
}

void QueryExecuter::executeDoInterventions(){
	for (int index = 0; index<doInterventionValues_.size(); index++){
		interventions_.doIntervention(doInterventionNodeID_[index], doInterventionValues_[doInterventionNodeID_[index]]);
	}
}

void QueryExecuter::executeReverseDoInterventions(){
	for (int index = 0; index<doInterventionValues_.size();index++){
		interventions_.reverseDoIntervention(doInterventionNodeID_[index], doInterventionValues_[doInterventionNodeID_[index]]);
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


void QueryExecuter::executeArgMax(){
	std::pair<float,std::vector<std::string>> result = probHandler_.maxSearch(argmaxNodeIDs_);
	std::cout<<"Probability: "<<result.first<<std::endl;
	std::cout<<"Argument: "<<result.second[0]<<std::endl;
}

void QueryExecuter::executeCondition(){
	std::vector<unsigned int> nominatorNodes = nonInterventionNodeID_;
	nominatorNodes.insert(nominatorNodes.end(), conditionNodeID_.begin(), conditionNodeID_.end());
	std::unordered_map<unsigned int, int> nominatorValues = nonInterventionValues_;
	nominatorValues.insert(conditionValues_.begin(), conditionValues_.end());

	float result = probHandler_.computeConditionalProbability(nominatorNodes, conditionNodeID_, nominatorValues, conditionValues_);
	std::cout<<result<<std::endl;
}

void QueryExecuter::executeProbability(){
	float result=probHandler_.computeJointProbability(nonInterventionNodeID_,nonInterventionValues_);
	std::cout<<result<<std::endl;
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
		os<<qe.nonInterventionNodeID_[index]<<" "<<qe.nonInterventionValues_.find(qe.nonInterventionNodeID_[index])->second<<std::endl;
	}
	os<<"Condition: "<<qe.conditionNodeID_.size()<<" "<<qe.conditionValues_.size()<<std::endl;
	for (unsigned int index = 0; index < qe.conditionNodeID_.size(); index++){
		os<<qe.conditionNodeID_[index]<<" "<<qe.conditionValues_.find(qe.conditionNodeID_[index])->second<<std::endl;
	}
	os<<"ArgMax: "<<qe.argmaxNodeIDs_.size()<<std::endl;
	for (unsigned int index = 0; index < qe.argmaxNodeIDs_.size(); index++){
		os<<qe.argmaxNodeIDs_[index]<<std::endl;
	}
	os<<"DoIntervention: "<<qe.doInterventionNodeID_.size()<<" "<<qe.doInterventionValues_.size()<<std::endl;
	for (unsigned int index = 0; index < qe.doInterventionNodeID_.size(); index++){
		os<<qe.doInterventionNodeID_[index]<<" "<<qe.doInterventionValues_.find(qe.doInterventionNodeID_[index])->second<<std::endl;
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

