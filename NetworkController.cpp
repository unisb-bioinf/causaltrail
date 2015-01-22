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
	Matrix<std::string> originalObservations (0,0,"NA"); 
	originalObservations.readMatrix(datafile,false,true,"NA");
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

bool NetworkController::isEdgePossible(unsigned int sourceID, unsigned int targetID){
	network_.addEdge(sourceID,targetID);
	bool edgePossible = network_.checkCycleExistence(sourceID);
	network_.removeEdge(sourceID,targetID);
	return edgePossible;
}
