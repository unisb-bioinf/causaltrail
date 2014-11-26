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
	void loadNetwork(const std::string& networkfile);
	void loadObservations(const std::string& datafile, const std::string& controlFile);
	void trainNetwork();
	const float& getLikelihoodOfTheData() const;
	Network& getNetwork();
	const int& getNumberOfEMRuns() const;
	const float& getParameterDifference() const;
	const int& getTimeInSeconds() const;
	private:
	Network network_;
	Matrix<int> observations_;
	int eMRuns_;
	float finalDifference_;
	float likelihoodOfTheData_;
	int timeInSeconds_;
};
#endif
