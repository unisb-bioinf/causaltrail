#include "Node.h"

Node::Node(unsigned int index, unsigned int id, const std::string& name)
	: index_(index),
	  id_(id),
	  name_(name),
	  DynProgMatrix_(0, 0, -1.0f),
	  observationRow_(-1),
	  parentCombinations_(0)
{
}

float Node::getProbability(unsigned int nv, unsigned int pv) const
{
	return ProbabilityMatrix_(nv, pv);
}

void Node::setProbability(float value, unsigned int nv, unsigned int pv)
{
	ProbabilityMatrix_.setData(value, nv, pv);
}

float Node::getProbability(const std::string& nv, const std::string& pv)
    const
{
	return ProbabilityMatrix_.getValueByNames(nv, pv);
}

void Node::setProbability(float value, const std::string& nv,
                          const std::string& pv)
{
	ProbabilityMatrix_.setData(value, ProbabilityMatrix_.findCol(nv),
	                           ProbabilityMatrix_.findRow(pv));
}

unsigned int Node::getObservations(unsigned int nv, unsigned int pv) const
{
	return ObservationMatrix_(nv, pv);
}

void Node::setObservations(int value, unsigned int nv, unsigned int pv)
{
	ObservationMatrix_.setData(value, nv, pv);
}

unsigned int Node::getObservations(const std::string& nv,
                                         const std::string& pv) const
{
	return ObservationMatrix_.getValueByNames(nv, pv);
}

void Node::setObservations(int value, const std::string& nv,
                           const std::string& pv)
{
	ObservationMatrix_.setData(value, ObservationMatrix_.findCol(nv),
	                           ObservationMatrix_.findRow(pv));
}
void Node::setProbability(const Matrix<float>& m) { ProbabilityMatrix_ = m; }

void Node::setObservations(const Matrix<int>& m) { ObservationMatrix_ = m; }

void Node::setObservationBackup(const Matrix<int>& m)
{
	ObservationMatrix_ = m;
}

const std::string& Node::getName() const { return name_; }

const unsigned int& Node::getIndex() const { return index_; }

const unsigned int& Node::getID() const { return id_; }

bool Node::hasValue(const std::string& v) const
{
	return ObservationMatrix_.findCol(v) > -1;
}

Matrix<float>& Node::getProbabilityMatrix() { return ProbabilityMatrix_; }

const Matrix<float>& Node::getProbabilityMatrix() const
{
	return ProbabilityMatrix_;
}

Matrix<int>& Node::getObservationMatrix() { return ObservationMatrix_; }

std::ostream& operator<<(std::ostream& os, const Node& n)
{
	os << "Node name: " << n.name_ << "\nNode id: " << n.id_
	   << "\nObservation matrix:\n" << n.ObservationMatrix_
	   << "\n\nProbability matrix:\n" << n.ProbabilityMatrix_ << std::endl;

	return os;
}

void Node::createBackup() { ObservationBackup_ = ObservationMatrix_; }

void Node::loadBackup() { ObservationMatrix_ = ObservationBackup_; }

const std::vector<unsigned int>& Node::getParents() const { return Parents_; }

size_t Node::getNumberOfParents() const {
	return Parents_.size();
}

void Node::setParents(const std::vector<unsigned int>& parents)
{
	Parents_ = parents;
	factor_.clear();
	factor_.resize(parents.size());
}

void Node::cutParents() { Parents_.clear(); parentValues_.clear(); factor_.clear();}

void Node::setUniqueValues(const std::vector<int>& uniqueValues)
{
	uniqueValues_ = uniqueValues;
}

const std::vector<int>& Node::getUniqueValues() const { return uniqueValues_; }

void Node::setUniqueValuesExcludingNA(const std::vector<int>& uniqueValues)
{
	uniqueValuesExcludingNA_ = uniqueValues;
}
std::vector<int>& Node::getUniqueValuesExcludingNA()
{
	return uniqueValuesExcludingNA_;
}

const std::vector<int>& Node::getUniqueValuesExcludingNA() const
{
	return uniqueValuesExcludingNA_;
}

size_t Node::getNumberOfUniqueValues() const
{
	return uniqueValues_.size();
}

size_t Node::getNumberOfUniqueValuesExcludingNA() const
{
	return uniqueValuesExcludingNA_.size();
}

size_t Node::getNumberOfParentValues() const
{
	return parentValueNames_.size();
}

void Node::setObservationRow(int row) { observationRow_ = row; }

int Node::getObservationRow() const { return observationRow_; }

void Node::setParentCombinations(int combinations)
{
	parentCombinations_ = combinations;
}

int Node::getParentCombinations() const { return parentCombinations_; }

void Node::setValueNames(const std::vector<std::string>& valueNames)
{
	valueNames_ = valueNames;
}

void Node::addValueName(const std::string& name)
{
	valueNames_.push_back(name);
}

const std::vector<std::string>& Node::getValueNames() const
{
	return valueNames_;
}

void Node::setValueNamesProb(const std::vector<std::string>& valueNames)
{
	valueNamesProb_ = valueNames;
}

void Node::addValueNameProb(const std::string& names)
{
	valueNamesProb_.push_back(names);
}

const std::vector<std::string>& Node::getValueNamesProb() const
{
	return valueNamesProb_;
}

void Node::setParentValueNames(const std::vector<std::string>& valueNames)
{
	parentValueNames_ = valueNames;
}

void Node::addParentValueName(const std::string& names)
{
	parentValueNames_.push_back(names);
}

const std::vector<std::string>& Node::getParentValueNames() const
{
	return parentValueNames_;
}

void Node::setUnvisited() { visited_ = false; }

void Node::visit() { visited_ = true; }

bool Node::isVisited() const { return visited_; }

void Node::createBackupDoIntervention()
{
	ProbabilityMatrixBackup_ = ProbabilityMatrix_;
	ParentsBackup_ = Parents_;
	factorBackup_ = factor_;
	revFactorBackup_ = revFactor_;
	parentValuesBackup_ = parentValues_;
}

void Node::loadBackupDoIntervention()
{
	ProbabilityMatrix_ = ProbabilityMatrixBackup_;
	Parents_ = ParentsBackup_;
	factor_ = factorBackup_;
	revFactor_ = revFactorBackup_;
	parentValues_ = parentValuesBackup_;
	parentValuesBackup_.clear();
	ParentsBackup_.clear();
	factorBackup_.clear();
	ProbabilityMatrixBackup_ = Matrix<float>(0, 0, 0.0);
}

void Node::setProbabilityTo1(const std::string& value)
{
	for(unsigned int col = 0; col < ProbabilityMatrix_.getColCount(); col++) {
		for(unsigned int row = 0; row < ProbabilityMatrix_.getRowCount();
		    row++) {
			ProbabilityMatrix_.setData(0.0f, col, row);
		}
	}
	unsigned int col = ProbabilityMatrix_.findCol(value);
	for(unsigned int row = 0; row < ProbabilityMatrix_.getRowCount(); row++) {
		ProbabilityMatrix_.setData(1.0f, col, row);

	}
}

void Node::setProbabilityTo1(int value)
{
	for(unsigned int col = 0; col < ProbabilityMatrix_.getColCount(); col++) {
		for(unsigned int row = 0; row < ProbabilityMatrix_.getRowCount();
		    row++) {
			ProbabilityMatrix_.setData(0.0f, col, row);
		}
	}
	for(unsigned int row = 0; row < ProbabilityMatrix_.getRowCount(); row++) {
		ProbabilityMatrix_.setData(1.0f, value, row);
	}
}

int Node::getIndex(const std::string& value) const
{
	return ProbabilityMatrix_.findCol(value);
}

void Node::clearNameVectors()
{
	uniqueValues_.clear();
	valueNames_.clear();
	valueNamesProb_.clear();
	parentValueNames_.clear();
	parentValues_.clear();
	uniqueValuesExcludingNA_.clear();
}

bool Node::isCalculated(unsigned int index, unsigned int row) const {
	return (DynProgMatrix_(index,row) != -1.0f); 
}

void Node::setCalculatedValue(float value, unsigned int index, unsigned int row){
	DynProgMatrix_.setData(value,index,row);
}

float Node::getCalculatedValue(unsigned int index, unsigned int row) const{
	return DynProgMatrix_(index,row);
}

void Node::clearDynProgMatrix(){
	DynProgMatrix_ = Matrix<float>(valueNamesProb_,parentValueNames_,-1.0f);
}

void Node::setParentValues(std::vector<std::vector<int>>& pValues){
	parentValues_ = pValues;
}

const std::vector<std::vector<int>>& Node::getParentValues() const{
	return parentValues_;
}

void Node::setName(std::string name){
	name_ = name;
}

void Node::setID(unsigned int id){
	id_ = id;
}

void Node::reset(){
	ProbabilityMatrix_ =  Matrix<float>(0, 0, 0.0f);
	ProbabilityMatrixBackup_ = Matrix<float>(0, 0, 0.0f);
    ObservationMatrix_ = Matrix<int>(0, 0, 0);
    ObservationBackup_ = Matrix<int>(0, 0, 0);
	DynProgMatrix_ =Matrix<float>(0, 0, -1.0f);
}

void Node::setFactor(unsigned int factor, unsigned int id){
	factor_[id]=factor;
}

unsigned int Node::getFactor(unsigned int id) const {
	return factor_[id];
}

void Node::setRevFactor(unsigned int factor, unsigned int row, unsigned int id){
	revFactor_[row][id]=factor;
}

unsigned int Node::getRevFactor(unsigned int row, unsigned int id) const {
	return revFactor_[row][id];
}

void Node::initialiseRevFactor(){
	std::vector<unsigned int> empty;
	empty.clear();
	empty.resize(getParents().size(),0);	
	revFactor_.clear();
	revFactor_.resize(ProbabilityMatrix_.getRowCount(),empty);
}
