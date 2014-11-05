#ifndef NETWORKCONTROLLER_H
#define NETWORKCONTROLLER_H
#include "Network.h"
#include "EM.h"
#include "Discretiser.h"
#include "DataDistribution.h"
#include <cmath>
#include <algorithm>
#include <utility>
#include <map>

class NetworkController{
	public:
	NetworkController();
	void loadNetwork(std::string networkfile);
	void loadObservations(std::string datafile,std::string controlFile);
	void trainNetwork();
	Network& getNetwork();
	private:
	Network network_;
	Matrix<int> observations_;
};
#endif
