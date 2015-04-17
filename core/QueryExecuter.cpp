#include "QueryExecuter.h"

QueryExecuter::QueryExecuter(NetworkController& c)
    : networkController_(c),
      probHandler_(c.getNetwork()),
      interventions_(c)
{
	size_t size = c.getNetwork().size();
	nonInterventionValues_.resize(size, -1);
	conditionValues_.resize(size, -1);
	doInterventionValues_.resize(size, -1);
}

bool QueryExecuter::hasInterventions()
{
	return doInterventionNodeID_.size() != 0 || addEdgeNodeIDs_.size() != 0 ||
	       removeEdgeNodeIDs_.size() != 0;
}

bool QueryExecuter::isCounterfactual()
{
	// Non internvetion ID in Condition
	for(auto& id : nonInterventionNodeID_) {
		if(std::find(conditionNodeID_.begin(), conditionNodeID_.end(), id) !=
		   conditionNodeID_.end()) {
			return true;
		}
	}
	// Do Intervention in Condition
	for(auto& id : doInterventionNodeID_) {
		if(std::find(conditionNodeID_.begin(), conditionNodeID_.end(), id) !=
		   conditionNodeID_.end()) {
			return true;
		}
	}
	return false;
}

void QueryExecuter::adaptNodeIdentifiers()
{
	size_t size = networkController_.getNetwork().size();
	nonInterventionValues_.resize(size, -1);
	conditionValues_.resize(size, -1);
	doInterventionValues_.resize(size, -1);
	for(unsigned int i = 0; i < nonInterventionNodeID_.size(); i++) {
		auto id = nonInterventionNodeID_[i];
		if(!networkController_.getNetwork()
		           .getNode(id)
		           .getParents()
		           .empty()) {
			nonInterventionNodeID_[i] =
			    networkController_.getNetwork().getHypoID(id);
			nonInterventionValues_[networkController_.getNetwork().getHypoID(
			    id)] = nonInterventionValues_[id];
			nonInterventionValues_[id] = -1;
		} else if(conditionValues_[id] != -1) {
			networkController_.getNetwork().removeHypoNodes();
			throw std::invalid_argument("It is not possible to infer and "
			                            "condition on exogenous variables "
			                            "simultaneously!");
		}
	}
	for(unsigned int j = 0; j < doInterventionNodeID_.size(); j++) {
		auto id = doInterventionNodeID_[j];
		if(!networkController_.getNetwork()
		           .getNode(id)
		           .getParents()
		           .empty()) {
			doInterventionNodeID_[j] =
			    networkController_.getNetwork().getHypoID(id);
			doInterventionValues_[networkController_.getNetwork().getHypoID(
			    id)] = doInterventionValues_[id];
			doInterventionValues_[id] = -1;
		}
		else if(conditionValues_[id] != -1){
			networkController_.getNetwork().removeHypoNodes();
			throw std::invalid_argument("It is not possible to set and "
										"condition on exogenous variables "
										"simultaneously!");	
		}
	}
}

std::pair<float, std::vector<std::string>> QueryExecuter::execute()
{
	if (nonInterventionNodeID_.empty() && argmaxNodeIDs_.empty()) {
		throw std::invalid_argument("A query can not be composed of interventions and conditions only!");
	}
	std::pair<float, std::vector<std::string>> probability;
	bool cf = false;
	if(isCounterfactual()) {
		if(!addEdgeNodeIDs_.empty() || !removeEdgeNodeIDs_.empty()) {
			throw std::invalid_argument("Edge additions and removals are not "
			                            "defined for counterfactuals");
		}
		networkController_.getNetwork().createTwinNetwork();
		cf = true;
		adaptNodeIdentifiers();
	}
	if(hasInterventions()) {
		executeInterventions();
	}
	probability = computeProbability();
	if(hasInterventions()) {
		reverseInterventions();
	}
	if(cf) {
		networkController_.getNetwork().removeHypoNodes();
	}
	return probability;
}

std::pair<float, std::vector<std::string>> QueryExecuter::computeProbability()
{
	std::vector<std::string> temp;
	if(!argmaxNodeIDs_.empty()) {
		return executeArgMax();
	} else if(!conditionNodeID_.empty()) {
		return std::make_pair(executeCondition(), temp);
	} else {
		return std::make_pair(executeProbability(), temp);
	}
}

void QueryExecuter::executeInterventions()
{
	networkController_.getNetwork().clearDynProgMatrices();
	interventions_.createBackupOfNetworkStructure();
	bool topologyChange = false;
	if(!removeEdgeNodeIDs_.empty()) {
		executeEdgeDeletions();
		topologyChange = true;
	}
	if(!addEdgeNodeIDs_.empty()) {
		executeEdgeAdditions();
		topologyChange = true;
	}
	if(topologyChange) {
		networkController_.trainNetwork();
	}
	if(!doInterventionValues_.empty()) {
		executeDoInterventions();
	}
}

void QueryExecuter::reverseInterventions()
{
	networkController_.getNetwork().clearDynProgMatrices();
	interventions_.loadBackupOfNetworkStructure();
	if(!doInterventionValues_.empty()) {
		executeReverseDoInterventions();
	}
	bool topologyChange = false;
	if(!addEdgeNodeIDs_.empty()) {
		executeEdgeAdditionsReverse();
		topologyChange = true;
	}
	if(!removeEdgeNodeIDs_.empty()) {
		executeEdgeDeletionsReverse();
		topologyChange = true;
	}
	if(topologyChange) {
		networkController_.trainNetwork();
	}
}

void QueryExecuter::executeDoInterventions()
{
	for(auto& id : doInterventionNodeID_) {
		interventions_.doIntervention(id, doInterventionValues_[id]);
	}
}

void QueryExecuter::executeReverseDoInterventions()
{
	for(auto& id : doInterventionNodeID_) {
		interventions_.reverseDoIntervention(id);
	}
}

void QueryExecuter::executeEdgeAdditions()
{
	for(auto& p : addEdgeNodeIDs_) {
		interventions_.addEdge(p.first, p.second);
		if(networkController_.getNetwork().checkCycleExistence(p.first)) {
			interventions_.removeEdge(p.first, p.second);
			throw std::invalid_argument("This edge induces a cycle");
		}
	}
}

void QueryExecuter::executeEdgeDeletions()
{
	for(auto& p : removeEdgeNodeIDs_) {
		interventions_.removeEdge(p.first, p.second);
	}
}

void QueryExecuter::executeEdgeAdditionsReverse()
{
	for(auto& p : addEdgeNodeIDs_) {
		interventions_.removeEdge(p.first, p.second);
	}
}

void QueryExecuter::executeEdgeDeletionsReverse()
{
	for(auto& p : removeEdgeNodeIDs_) {
		interventions_.addEdge(p.first, p.second);
	}
}

std::pair<float, std::vector<std::string>> QueryExecuter::executeArgMax()
{
	return probHandler_.maxSearch(argmaxNodeIDs_, conditionNodeID_,
	                              conditionValues_);
}

float QueryExecuter::executeCondition()
{
	return probHandler_.computeConditionalProbability(
	    nonInterventionNodeID_, conditionNodeID_, nonInterventionValues_,
	    conditionValues_);
}

float QueryExecuter::executeProbability()
{
	if(nonInterventionNodeID_.size() == 1) {
		return probHandler_.computeTotalProbabilityNormalized(
		    nonInterventionNodeID_[0],
		    nonInterventionValues_[(nonInterventionNodeID_[0])]);
	} else {
		return probHandler_.computeJointProbabilityUsingVariableElimination(
		    nonInterventionNodeID_, nonInterventionValues_);
	}
}

void QueryExecuter::setNonIntervention(const unsigned int nodeID,
                                       const unsigned int valueID)
{
	if(std::find(nonInterventionNodeID_.begin(), nonInterventionNodeID_.end(),
	             nodeID) == nonInterventionNodeID_.end()) {
		nonInterventionNodeID_.push_back(nodeID);
		nonInterventionValues_[nodeID] = valueID;
	} else {
		throw std::invalid_argument("Nodes can not occur multiple times in the "
		                            "non intervention node list");
	}
}

void QueryExecuter::setCondition(const unsigned int nodeID,
                                 const unsigned int valueID)
{
	conditionNodeID_.push_back(nodeID);
	conditionValues_[nodeID] = valueID;
}

void QueryExecuter::setDoIntervention(const unsigned int nodeID,
                                      const unsigned int valueID)
{
	doInterventionNodeID_.push_back(nodeID);
	doInterventionValues_[nodeID] = valueID;
}

void QueryExecuter::setAddEdge(const unsigned int nodeID1,
                               const unsigned int nodeID2)
{
	addEdgeNodeIDs_.push_back(std::make_pair(nodeID1, nodeID2));
}

void QueryExecuter::setRemoveEdge(const unsigned int nodeID1,
                                  const unsigned int nodeID2)
{
	removeEdgeNodeIDs_.push_back(std::make_pair(nodeID1, nodeID2));
}

void QueryExecuter::setArgMax(const unsigned int nodeID)
{
	argmaxNodeIDs_.push_back(nodeID);
}

std::ostream& operator<<(std::ostream& os, const QueryExecuter& qe)
{
	os << "NonIntervention: " << qe.nonInterventionNodeID_.size() << " "
	   << qe.nonInterventionValues_.size() << std::endl;
	for(unsigned int index = 0; index < qe.nonInterventionNodeID_.size();
	    index++) {
		os << qe.nonInterventionNodeID_[index] << " "
		   << qe.nonInterventionValues_[qe.nonInterventionNodeID_[index]]
		   << std::endl;
	}
	os << "Condition: " << qe.conditionNodeID_.size() << " "
	   << qe.conditionValues_.size() << std::endl;
	for(unsigned int index = 0; index < qe.conditionNodeID_.size(); index++) {
		os << qe.conditionNodeID_[index] << " "
		   << qe.conditionValues_[(qe.conditionNodeID_[index])] << std::endl;
	}
	os << "ArgMax: " << qe.argmaxNodeIDs_.size() << std::endl;
	for(unsigned int index = 0; index < qe.argmaxNodeIDs_.size(); index++) {
		os << qe.argmaxNodeIDs_[index] << std::endl;
	}
	os << "DoIntervention: " << qe.doInterventionNodeID_.size() << " "
	   << qe.doInterventionValues_.size() << std::endl;
	for(unsigned int index = 0; index < qe.doInterventionNodeID_.size();
	    index++) {
		os << qe.doInterventionNodeID_[index] << " "
		   << qe.doInterventionValues_[(qe.doInterventionNodeID_[index])]
		   << std::endl;
	}
	os << "AddEdge: " << qe.addEdgeNodeIDs_.size() << std::endl;
	for(unsigned int index = 0; index < qe.addEdgeNodeIDs_.size(); index++) {
		os << qe.addEdgeNodeIDs_[index].first << " "
		   << qe.addEdgeNodeIDs_[index].second << std::endl;
	}
	os << "RemoveEdge: " << qe.removeEdgeNodeIDs_.size() << std::endl;
	for(unsigned int index = 0; index < qe.removeEdgeNodeIDs_.size(); index++) {
		os << qe.removeEdgeNodeIDs_[index].first << " "
		   << qe.removeEdgeNodeIDs_[index].second << std::endl;
	}

	return os;
}

