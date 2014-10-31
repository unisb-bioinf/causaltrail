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
	void performEM();
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
	float calculateProbabilityEM(Node& n, unsigned int col, unsigned int row);
	std::vector<float> createSortedVector(unsigned int row);
	void ePhase();
	float mPhase();
	void initialise(unsigned int method);
	void initialise1();
	void initialise2();
	void initialise3();
	std::vector<std::string> split(std::string& str, char delim);
	float computeTotalProbability(unsigned int key, std::string value);
/*	float computeConditionalProbability(std::vector<unsigned int> qNodes, std::vector<std::string> qValues, std::vector<unsigned int> oNodes, std::vector<std::string>oValues);
	float computeJointProbability(std::vector<unsigned int> qNodes, std::vector<std::string> qValue);
	void doIntervention(unsigned int key, std::string value);
	void addRelation(unsigned int nodeID1, unsigned int nodeID2);
	void removeRelation(unsigned int nodeID1, unsigned int nodeID2);*/
	Network network_;
	Matrix<std::string> originalObservations_;
	Matrix<int> observations_;
	std::unordered_map<std::string,int> observationsMap_;
    std::map<std::pair<int,int>, std::string> observationsMapR_;
};
#endif
