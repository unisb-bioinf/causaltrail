#ifndef EM_H
#define EM_H
#include "ProbabilityHandler.h"
#include <boost/algorithm/string/split.hpp>
#include <cmath>
#include <chrono>

class EM{
	public:
	EM(Network& network, Matrix<int>& observations_,float differenceThreshold_ = 0.0001f, unsigned int maxRuns_=10000);	
	void performEM();
	float calculateLikelihoodOfTheData();
	int getNumberOfRuns();
	float getDifference();
	int getTimeInSeconds();
	private:
	float calculateProbabilityEM(const Node& n, unsigned int col, unsigned int row);
	void calculateExpectedValue(unsigned int row, const Node& n, Matrix<int>& obMatrix);
	void ePhase();
	void calculateMaximumLikelihood(unsigned int row, unsigned int& counter, float& difference, Node& n, const Matrix<int> & obMatrix);
	float mPhase();
	void initalise();
	void initalise1();
	void initalise2();
	void initalise3();
	Network& network_;
	unsigned int method_;
	Matrix<int>& observations_;
	ProbabilityHandler probHandler_;
	float differenceThreshold_;
	unsigned int maxRuns_;
	int neededRuns_;
	float finalDifference_;
	std::chrono::time_point<std::chrono::system_clock> start;
	std::chrono::time_point<std::chrono::system_clock> end;
};

#endif
