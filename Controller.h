#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Network.h"
#include "EM.h"
#include "Discretiser.h"
#include "ObservationHandler.h"
#include <cmath>
#include <algorithm>
#include <utility>
#include <map>

class Controller{
	public:
	Controller();
	void loadNetwork(std::string networkfile);
	void discretise(std::string datafile,std::string controlFile);
	void trainNetwork();
	Network& getNetwork();
	private:
	Network network_;
	Matrix<std::string> originalObservations_;
	Matrix<int> observations_;
	std::unordered_map<std::string,int> observationsMap_;
    std::map<std::pair<int,int>, std::string> observationsMapR_;
};
#endif
