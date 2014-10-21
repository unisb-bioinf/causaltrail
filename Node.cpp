#include "Node.h"

Node::Node(unsigned int index, unsigned int id, std::string name)
	:index_(index),id_(id),name_(name),ProbabilityMatrix_(Matrix<float>(0,0,0.0)),ObservationMatrix_(Matrix<int>(0,0,0))
	{
	}
 
float Node::getProbability(unsigned int nv, unsigned int pv){
	return ProbabilityMatrix_(nv,pv);
	}
 
float Node::getProbability(std::string nv, std::string pv){
	return ProbabilityMatrix_.getValueByNames(nv,pv);
	}

unsigned int Node::getObservations(unsigned int nv, unsigned int pv){
	return ObservationMatrix_(nv,pv);
	}
  
unsigned int Node::getObservations(std::string nv, std::string pv){
	return ObservationMatrix_.getValueByNames(nv,pv);
	}
  
void Node::setProbability(Matrix<float> m){
	ProbabilityMatrix_=m;
	}
  
void Node::setObservations(Matrix<int> m){
	ObservationMatrix_=m;
	}

std::string& Node::getName(){
	return name_;
	}

unsigned int&  Node::getIndex(){
	return index_;
	}

unsigned int&  Node::getID(){
	return id_;
	}

bool Node::hasValue(std::string v){
	if (ObservationMatrix_.findRow(v)>-1)
		return true;
	return false;
	}

const Matrix<float>& Node::getProbabilityMatrix(){
	return ProbabilityMatrix_;
	}

const Matrix<int>& Node::getObservationMatrix(){
	return ObservationMatrix_;
	}

std::ostream& operator<<(std::ostream& os,const Node& n){
	os<<n.name_<<" "<<n.id_<<std::endl;
	return os;
	}
