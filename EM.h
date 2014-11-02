#ifndef EM_H
#define EM_H
#include "Network.h"
#include <cmath>

class EM{
	public:
	EM(unsigned int method, Network& network, Matrix<int>& observations_);
	void performEM();
	private:
	float calculateProbabilityEM(Node& n, unsigned int col, unsigned int row);
	float computeTotalProbability(unsigned int key, std::string value);
	std::vector<std::string> split(std::string& str, char delim);
	void ePhase();
	float mPhase();
	void initalise();
	void initalise1();
	void initalise2();
	void initalise3();
	Network& network_;
	unsigned int method_;
	Matrix<int>& observations_;
};

#endif
