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
void NetworkController::loadNetwork(std::string networkfile){
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

void NetworkController::loadObservations(std::string datafile, std::string controlFile){
	Discretiser disc = Discretiser(datafile, controlFile, observations_, network_);
}

/**trainNetwork
 *
 * @return void
 *
 * Calls the appropiate methods to train the model.
 */

void NetworkController::trainNetwork(){
	DataDistribution datadu= DataDistribution(network_, observations_);
	EM em = EM(0,network_,observations_);
}

