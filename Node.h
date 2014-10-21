#ifndef NODE_H
#define NODE_H
#include "Matrix.h"

class Node {
	public:
	Node(unsigned int index,unsigned int id, std::string name);
	float getProbability(unsigned int nv, unsigned int pv);
	float getProbability(std::string nv, std::string pv);
	unsigned int getObservations(unsigned int nv, unsigned int pv);
	unsigned int getObservations(std::string nv, std::string pv);
	void setProbability(Matrix<float> m);	
	void setObservations(Matrix<int> m);
	std::string& getName();
	unsigned int& getIndex();
	unsigned int& getID();
	bool hasValue(std::string v);
	const Matrix<float>& getProbabilityMatrix();
	const Matrix<int>& getObservationMatrix();
	friend std::ostream& operator<< (std::ostream& os,const Node& n);
	private:
	unsigned int index_;
	unsigned int id_;
	std::string name_;
	Matrix<float> ProbabilityMatrix_;
	Matrix<int> ObservationMatrix_;
};
#endif
