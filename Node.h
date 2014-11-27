#ifndef NODE_H
#define NODE_H
#include "Matrix.h"

class Node {
	public:
	Node(unsigned int index,unsigned int id, const std::string& name);
	const float getProbability(unsigned int nv, unsigned int pv) const;
	const float getProbability(const std::string& nv, const std::string& pv) const;	
	void setProbability(float value, unsigned int nv, unsigned int pv);
	void setProbability(float value, const std::string& nv, const std::string& pv);
	const unsigned int getObservations(unsigned int nv, unsigned int pv) const ;
	const unsigned int getObservations(const std::string& nv, const std::string& pv) const ;	
	void setObservations(int value, unsigned int nv, unsigned int pv);
	void setObservations(int value, const std::string& nv, const std::string& pv);
	void setProbability(const Matrix<float>& m);	
	void setProbabilityTo1(const std::string& value);
	void setProbabilityTo1(int value);
	void createBackupDoIntervention();
	void loadBackupDoIntervention();
	void setObservations(const Matrix<int>& m);
	void setObservationBackup(const Matrix<int>& m);
	const std::string& getName() const;
	const unsigned int& getIndex() const;
	const unsigned int& getID() const;
	const int getIndex(const std::string& value) const;
	bool hasValue(const std::string& v) const;
	const std::vector<unsigned int>& getParents() const;
	void setParents(const std::vector<unsigned int>& parents);
	void cutParents();
	void setUniqueValues(const std::vector<int>& uniqueValues);
	const std::vector<int>& getUniqueValues() const;
	void setUniqueValuesExcludingNA(const std::vector<int>&);
	std::vector<int>& getUniqueValuesExcludingNA();
	const std::vector<int>& getUniqueValuesExcludingNA() const;
	const unsigned int getNumberOfUniqueValues() const;
	const unsigned int getNumberOfUnqiueValuesExcludingNA() const;
	void setObservationRow(int row);
	const int getObservationRow() const;
	void setParentCombinations(int row);
	const int getParentCombinations() const;
	void setValueNames(const std::vector<std::string>& valueNames);
	void addValueName(const std::string& name);
	const std::vector<std::string>& getValueNames() const;
	void setValueNamesProb(const std::vector<std::string>& valueNames);
	void addValueNameProb(const std::string& name);
	const std::vector<std::string>& getValueNamesProb() const;
	void setParentValueNames(const std::vector<std::string>& valueNames); 
	void addParentValueName(const std::string& name);
	const std::vector<std::string>& getParentValueNames() const;
	Matrix<float>& getProbabilityMatrix();
	const Matrix<float>& getProbabilityMatrix() const;
	Matrix<int>& getObservationMatrix();
	const Matrix<int>& getObservationMatrix() const;
	void createBackup();
	void loadBackup();
	void setUnvisited();
	void visit();
	bool isVisited() const;
	void clearNameVectors();
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
