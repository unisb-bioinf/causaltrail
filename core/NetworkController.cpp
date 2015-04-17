#include "NetworkController.h"

NetworkController::NetworkController()
	:network_(Network()),observations_(Matrix<int>(0,0,-1))
	{
}

void NetworkController::loadNetwork(const std::string& networkfile){
	network_.readNetwork(networkfile);
}

Network& NetworkController::getNetwork(){
	return network_;
}

void NetworkController::loadObservations(const std::string& datafile, const std::string& controlFile){
	Matrix<std::string> originalObservations (datafile,false,true); 
	Discretiser disc = Discretiser(originalObservations, controlFile, observations_, network_);
}

void NetworkController::loadObservations(const std::string& datafile, const std::string& controlFile, const std::vector<unsigned int>& samplesToDelete){
    Matrix<std::string> originalObservations (datafile,false,true, samplesToDelete);
    Discretiser disc = Discretiser(originalObservations, controlFile, observations_, network_);
}


void NetworkController::trainNetwork(){
	DataDistribution datadu= DataDistribution(network_, observations_);
	datadu.assignObservationsToNodes();
	datadu.distributeObservations();
	EM em = EM(network_,observations_,0.001,100000);
	eMRuns_ = em.getNumberOfRuns();
	finalDifference_ = em.getDifference();
	likelihoodOfTheData_ = em.calculateLikelihoodOfTheData();
	timeInMicroSeconds_ = em.getTimeInMicroSeconds();
	network_.clearDynProgMatrices();
}

const float& NetworkController::getLikelihoodOfTheData() const {
	return likelihoodOfTheData_;
}

const int& NetworkController::getNumberOfEMRuns() const{
	return eMRuns_;
}

const float& NetworkController::getParameterDifference() const{
	return finalDifference_;
}

const int& NetworkController::getTimeInMicroSeconds() const{
	return timeInMicroSeconds_;
}

void NetworkController::saveParameters() const{
	network_.saveParameters();
}

bool NetworkController::isEdgePossible(unsigned int sourceID, unsigned int targetID, std::vector<std::pair<unsigned int, unsigned int>>& addedEdges,std::vector<std::pair<unsigned int, unsigned int>>& removedEdges){
	for (auto& pair : addedEdges){
		network_.addEdge(pair.second,pair.first);
	}
	for (auto& pair : removedEdges){
		network_.removeEdge(pair.second,pair.first);
	}
	network_.addEdge(targetID,sourceID);
	bool edgePossible = !network_.checkCycleExistence(sourceID);
	network_.removeEdge(targetID,sourceID);
	for (auto& pair : removedEdges){
		network_.addEdge(pair.second,pair.first);
	}
	for (auto& pair : addedEdges){
		network_.removeEdge(pair.second,pair.first);
	}
	return edgePossible;
}
