#ifndef EM_H
#define EM_H
#include "ProbabilityHandler.h"
#include <boost/algorithm/string/split.hpp>
#include <cmath>

class EM{
	public:
	EM(Network& network, Matrix<int>& observations_,float differenceThreshold_ = 0.0001f, unsigned int maxRuns_=10000);
	void performEM();
	private:
	float calculateProbabilityEM(Node& n, unsigned int col, unsigned int row);
	void calculateExpectedValue(unsigned int row, Node& n, Matrix<int>& obMatrix);
	void ePhase();
	void calculateMaximumLikelihood(unsigned int row, unsigned int& counter, float& difference, Node& n, const Matrix<int> & obMatrix);
	float mPhase();
	void initalise();
	void initalise1();
	void initalise2();
	void initalise3();
	std::unordered_map<unsigned int, int> loadSample(std::vector<unsigned int> nodes, unsigned int sample);
	float calculateLikelihoodOfTheData();
	Network& network_;
	unsigned int method_;
	Matrix<int>& observations_;
	ProbabilityHandler probHandler_;
	float differenceThreshold_;
	unsigned int maxRuns_;
};

#endif
