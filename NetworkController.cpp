#include "NetworkController.h"

/**Constructor
*
* @return A Controller Object
* 
*/
NetworkController::NetworkController()
	:network_(Network()),observations_(Matrix<int>(0,0,0))
	{
}

/**loadNetwork
 *
 * @param filename name of the file containing the (partial) network structure
 *
 * @return void
 *
 */
void NetworkController::loadNetwork(const std::string& networkfile){
	network_.readNetwork(networkfile);
}

/**getNetwork
 *
 *
 *
 * @return returns the constructed network object
 */

Network& NetworkController::getNetwork(){
	return network_;
}

void NetworkController::loadObservations(const std::string& datafile, const std::string& controlFile){
	Matrix<std::string> originalObservations (0,0,"NA"); 
	originalObservations.readMatrix(datafile,false,true,"NA");
	Discretiser disc = Discretiser(originalObservations, controlFile, observations_, network_);
}

/**trainNetwork
 *
 * @return void
 *
 * Calls the appropiate methods to train the model.
 */

void NetworkController::trainNetwork(){
	DataDistribution datadu= DataDistribution(network_, observations_);
	EM em = EM(network_,observations_,0.001,100000);
	eMRuns_ = em.getNumberOfRuns();
	finalDifference_ = em.getDifference();
	likelihoodOfTheData_ = em.calculateLikelihoodOfTheData();
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
