#include "Node.h"

/**Detailed Constructor
 *
 * @param index position in the NodeList_ and the AdjacencyMatrix_ of the Network class
 * @param id identifier of the node
 * @param name name of the node
 *
 * @return Node object
 * 
 *Creates a Node using its position in the nodelist(index), a node identifier and a node name
 */
Node::Node(unsigned int index, unsigned int id, std::string name)
	:index_(index),id_(id),name_(name),ProbabilityMatrix_(Matrix<float>(0,0,0.0)),ObservationMatrix_(Matrix<int>(0,0,0)),ObservationBackup_(Matrix<int>(0,0,0))
	{
	}

/**getProbability
 *
 * @param index of the value of the current node 
 * @param index of the parent values
 *
 * @return Probability
 * 
 *Returns the probability for the event definded by nv and pv
 */
float Node::getProbability(unsigned int nv, unsigned int pv){
	return ProbabilityMatrix_(nv,pv);
	}


void Node::setProbability(float value, unsigned int nv, unsigned int pv){
	ProbabilityMatrix_.setData(value,nv,pv);
	}
 
/**getProbability
 *
 * @param name of the value of the current node
 * @param name of the parent values
 *
 * @return Probability
 * 
 *Returns the probability for the event defined by nv and pv
 */
float Node::getProbability(std::string nv, std::string pv){
	return ProbabilityMatrix_.getValueByNames(nv,pv);
	}

void Node::setProbability(float value, std::string nv, std::string pv){	
	 ProbabilityMatrix_.setData(value,ProbabilityMatrix_.findCol(nv),ProbabilityMatrix_.findRow(pv));
	}


/**getObservations
 *
 * @param index of the value of the current node
 * @param index of the parent values
 *
 * @return Observations
 * 
 *Returns the number of occurences of the event defined by nv and pv
 */
unsigned int Node::getObservations(unsigned int nv, unsigned int pv){
	return ObservationMatrix_(nv,pv);
	}

void Node::setObservations(int value, unsigned int nv, unsigned int pv){
	ObservationMatrix_.setData(value, nv, pv);
	}
  
/**getObservations
 *
 * @param name of the value of the current node
 * @param name of the parent values
 *
 * @return Observations
 * 
 *Returns the number of occurences of the event defined by nv and pv
 */
unsigned int Node::getObservations(std::string nv, std::string pv){
	return ObservationMatrix_.getValueByNames(nv,pv);
	}
 
void Node::setObservations(int value, std::string nv, std::string pv){
	ObservationMatrix_.setData(value,ObservationMatrix_.findCol(nv),ObservationMatrix_.findRow(pv));
	} 
	
/**setProbability()
 *
 * @param m A matrix of type float containing Probabilities
 *
 * @return void
 * 
 *
 */
void Node::setProbability(Matrix<float> m){
	ProbabilityMatrix_=m;
	}
  
/**setObservations
 *
 * @param m A matrix of type int containing counts for observations 
 *
 * @return
 * 
 *
 */
void Node::setObservations(Matrix<int> m){
	ObservationMatrix_=m;
	}


void Node::setObservationBackup(Matrix<int> m){
	ObservationMatrix_=m;
	}

/**getName
 *
 * @return name of the node
 * 
 *
 */
std::string& Node::getName(){
	return name_;
	}

/**getIndex
 *
 * @return index of the node
 * 
 *
 */
unsigned int&  Node::getIndex(){
	return index_;
	}

/**getID
 *
 * @return identifier of the node
 * 
 *
 */
unsigned int&  Node::getID(){
	return id_;
	}

/**hasValue
 *
 * @param query element v
 *
 * @return true if v is a possible value of the current node, false otherwise
 * 
 *Checks whether v is a value of the current node
 */
bool Node::hasValue(std::string v){
	if (ObservationMatrix_.findCol(v)>-1)
		return true;
	return false;
	}

/**getProbabilityMatrix
 *
 * @return A reference to the probability matrix of the current node
 * 
 *
 */
Matrix<float>& Node::getProbabilityMatrix(){
	return ProbabilityMatrix_;
	}

/**getObservationMatrix
 *
 * @return A reference to the observation matrix of the current node
 * 
 *
 */
Matrix<int>& Node::getObservationMatrix(){
	return ObservationMatrix_;
	}

/**operator<<
 *
 * @param ostream
 * @param n
 *
 * @return ostream
 * 
 * Outputs the name, the identifier and the probability matrix of the current node
 */
std::ostream& operator<<(std::ostream& os,const Node& n){
	os<<n.name_<<" "<<n.id_<<"\n"<<n.ObservationMatrix_<<"\n"<<n.ProbabilityMatrix_<<std::endl;
	return os;
	}

void Node::createBackup(){
	ObservationBackup_=ObservationMatrix_;
	}

void Node::loadBackup(){
	ObservationMatrix_=ObservationBackup_;
	}
