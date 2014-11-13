#ifndef NODE_H
#define NODE_H
#include "Matrix.h"

class Node {
	public:
	Node(unsigned int index,unsigned int id, std::string name);
	float getProbability(unsigned int nv, unsigned int pv);
	float getProbability(std::string nv, std::string pv);
	void setProbability(float value, unsigned int nv, unsigned int pv);
	void setProbability(float value, std::string nv, std::string pv);
	unsigned int getObservations(unsigned int nv, unsigned int pv);
	unsigned int getObservations(std::string nv, std::string pv);
	void setObservations(int value, unsigned int nv, unsigned int pv);
	void setObservations(int value, std::string nv, std::string pv);
	void setProbability(Matrix<float> m);	
	void setProbabilityTo1(std::string value);
	void createBackupDoIntervention();
	void loadBackupDoIntervention();
	void setObservations(Matrix<int> m);
	void setObservationBackup(Matrix<int> m);
	std::string& getName();
	unsigned int& getIndex();
	unsigned int& getID();
	const unsigned int& getID() const;
	bool hasValue(std::string v);
	std::vector<unsigned int>& getParents();
	const std::vector<unsigned int>& getParents() const;
	void setParents(std::vector<unsigned int> parents);
	void cutParents();
	void setUniqueValues(std::vector<int> uniqueValues);
	std::vector<int>& getUniqueValues();
	void setUniqueValuesExcludingNA(std::vector<int>);
	std::vector<int>& getUniqueValuesExcludingNA();
	void setObservationRow(int row);
	int getObservationRow();
	void setParentCombinations(int row);
	int getParentCombinations();
	void setValueNames(std::vector<std::string> valueNames);
	void addValueName(std::string name);
	std::vector<std::string>& getValueNames();
	void setValueNamesProb(std::vector<std::string> valueNames);
	void addValueNameProb(std::string name);
	std::vector<std::string>& getValueNamesProb();
	void setParentValueNames(std::vector<std::string> valueNames);
	void addParentValueName(std::string name);
	std::vector<std::string>& getParentValueNames();		
	Matrix<float>& getProbabilityMatrix();
	const Matrix<float>& getProbabilityMatrix() const;
	int getIndex(std::string value);
	Matrix<int>& getObservationMatrix();
	void createBackup();
	void loadBackup();
	void setUnvisited();
	void visit();
	bool isVisited();
	friend std::ostream& operator<< (std::ostream& os,const Node& n);
	private:
	unsigned int index_;
	unsigned int id_;
	std::vector<unsigned int> Parents_;
	std::vector<unsigned int> ParentsBackup_;
	std::string name_;
	Matrix<float> ProbabilityMatrix_;
	Matrix<float> ProbabilityMatrixBackup_;
	Matrix<int> ObservationMatrix_;
	Matrix<int> ObservationBackup_;
	std::vector<int> uniqueValues_;
	std::vector<std::string> valueNames_;
	std::vector<std::string> valueNamesProb_;
	std::vector<std::string> parentValueNames_;
	std::vector<int> uniqueValuesExcludingNA_;
	int observationRow_;
	int parentCombinations_;
	bool visited_=false;
};
#endif
