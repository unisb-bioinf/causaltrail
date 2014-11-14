#ifndef DISCRETISER_H
#define DISCRETISER_H

#include "Network.h"
#include <map>

class Discretiser{
	public: 
	Discretiser(Matrix<std::string>& originalObservations, Matrix<int>& obsMatrix, Network& network);
	Discretiser(Matrix<std::string>& originalObservations, std::string filename, Matrix<int>& obsMatrix, Network& network);
	void discretiseRow(unsigned int row, unsigned int method, float threshold);
	private:
	void discretise(std::string filename);
	void discretiseFloor(unsigned int row);
	void discretiseCeil(unsigned int row);
	void discretiseRound(unsigned int row);
	void discretiseByAMean(unsigned int row);
	void discretiseByHMean(unsigned int row);
	void discretiseByMedian(unsigned int row);
	void discretiseManually(unsigned int row, float threshold);
	void discretiseBracketMedians(unsigned int row, unsigned int numberOfBrackets);
	void discretisePearsonTukey(unsigned int row);
	void mapNamesToInt(unsigned int row);
	float getNumber(unsigned int col, unsigned int row);
	void convertToDenseNumbers(unsigned int row);
	void createNameEntry(int value, unsigned int row);
	void adaptFormat();
	std::vector<float> createSortedVector(unsigned int row);
	Matrix<std::string> originalObservations_;
	Matrix<int>& observations_;
	std::unordered_map<std::string,int>& observationsMap_;
    std::map<std::pair<int,int>, std::string>& observationsMapR_;
};
#endif
