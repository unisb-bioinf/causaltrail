#include "Controller.h"

/**Constructor
*
* @return A Controller Object
* 
*/
Controller::Controller()
	:network_(Network()),originalObservations_(Matrix<std::string>(0,0,"NA")),observations_(Matrix<int>(0,0,0))
	{
}

/**loadNetwork
 *
 * @param filename name of the file containing the (partial) network structure
 *
 * @return void
 *
 */
void Controller::loadNetwork(std::string networkfile){
	network_.readNetwork(networkfile);
}

/**getNetwork
 *
 *
 *
 * @return returns the constructed network object
 */

Network& Controller::getNetwork(){
	return network_;
}

void Controller::discretise(std::string datafile, std::string controlFile){
	Discretiser disc = Discretiser(datafile, controlFile, observations_, observationsMap_, observationsMapR_);
	ObservationHandler obsh= ObservationHandler(network_, observations_, observationsMap_, observationsMapR_);
	
}


/**trainNetwork
 *
 * @return void
 *
 * Calls the appropiate methods to train the model.
 */

void Controller::trainNetwork(){
	EM em = EM(0,network_,observations_);
	em.performEM();
}

