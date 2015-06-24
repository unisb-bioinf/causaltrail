#include "NetworkController.h"

#include "DataDistribution.h"
#include "Discretiser.h"
#include "DiscretisationSettings.h"
#include "EM.h"

NetworkController::NetworkController()
    : observations_(0, 0, -1),
      eMRuns_(0),
      finalDifference_(0),
      likelihoodOfTheData_(0.0f),
      timeInMicroSeconds_(0)
{
}

void NetworkController::loadNetwork(const std::string& networkfile){
	network_.readNetwork(networkfile);
}

Network& NetworkController::getNetwork(){
	return network_;
}

const Network& NetworkController::getNetwork() const {
	return network_;
}

void NetworkController::loadObservations(const std::string& datafile,
                                         const std::string& controlFile)
{
	Matrix<std::string> originalObservations(datafile, false, true);
	Discretiser d(originalObservations,controlFile,observations_,network_);
}

void NetworkController::loadObservations(
    const std::string& datafile, const std::string& controlFile,
    const std::vector<unsigned int>& samplesToDelete)
{
	Matrix<std::string> originalObservations(datafile, false, true,samplesToDelete);
	Discretiser d(originalObservations,controlFile,observations_,network_);
}

void NetworkController::loadObservations(
	const std::string& datafile, 
	DiscretisationSettings& propertyTree)
{
	Matrix<std::string> originalObservations(datafile, false, true);
	Discretiser d(originalObservations,observations_,network_);
	d.setJsonTree(propertyTree);
	d.discretise();
}

void NetworkController::loadObservations(
	const std::string& datafile, 
	DiscretisationSettings& propertyTree,
	const std::vector<unsigned int>& samplesToDelete)
{
	Matrix<std::string> originalObservations(datafile, false, true,samplesToDelete);
	Discretiser d(originalObservations,observations_,network_);
	d.setJsonTree(propertyTree);
	d.discretise();
}



void NetworkController::trainNetwork(){
	DataDistribution datadu(network_, observations_);
	datadu.assignObservationsToNodes();
	datadu.distributeObservations();
	EM em(network_, observations_, 0.001f, 100000);
	eMRuns_ = em.getNumberOfRuns();
	finalDifference_ = em.getDifference();
	likelihoodOfTheData_ = em.calculateLikelihoodOfTheData();
	timeInMicroSeconds_ = em.getTimeInMicroSeconds();
	network_.clearDynProgMatrices();
}

float NetworkController::getLikelihoodOfTheData() const {
	return likelihoodOfTheData_;
}

int NetworkController::getNumberOfEMRuns() const {
	return eMRuns_;
}

float NetworkController::getParameterDifference() const {
	return finalDifference_;
}

int NetworkController::getTimeInMicroSeconds() const {
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
