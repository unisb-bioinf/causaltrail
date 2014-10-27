#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Network.h"
#include "Combinations.h"
#include <utility>
#include<map>
class Controller{
	public:
	Controller();
	void loadNetwork(std::string networkfile);
	void discretise(std::string datafile,std::string controlFile);
    void distributeObservations();
	private:
	void discretiseRow(unsigned int row, unsigned int method, float threshold);
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
	void createNameEntry(int value, unsigned int row);
	std::vector<float> createSortedVector(unsigned int row);
	Network network_;
	Matrix<std::string> originalObservations_;
	Matrix<int> observations_;
	std::unordered_map<std::string,int> observationsMap_;
    std::map<std::pair<int,int>, std::string> observationsMapR_;
};
#endif
