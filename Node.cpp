#include "Node.h"

Node::Node(unsigned int index, unsigned int id, std::string name)
    : index_(index),
      id_(id),
      name_(name),
      ProbabilityMatrix_(Matrix<float>(0, 0, 0.0f)),
      ProbabilityMatrixBackup_(Matrix<float>(0, 0, 0.0f)),
      ObservationMatrix_(Matrix<int>(0, 0, 0)),
      ObservationBackup_(Matrix<int>(0, 0, 0))
{
}

const float Node::getProbability(unsigned int nv, unsigned int pv) const
{
	return ProbabilityMatrix_(nv, pv);
}

void Node::setProbability(float value, unsigned int nv, unsigned int pv)
{
	ProbabilityMatrix_.setData(value, nv, pv);
}

const float Node::getProbability(const std::string& nv, const std::string& pv)
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

const unsigned int Node::getObservations(unsigned int nv, unsigned int pv) const
{
	return ObservationMatrix_(nv, pv);
}

void Node::setObservations(int value, unsigned int nv, unsigned int pv)
{
	ObservationMatrix_.setData(value, nv, pv);
}

const unsigned int Node::getObservations(const std::string& nv,
                                         const std::string& pv) const
{
	return ObservationMatrix_.getValueByNames(nv, pv);
}

void Node::setObservations(int value, std::string nv, std::string pv)
{
	ObservationMatrix_.setData(value, ObservationMatrix_.findCol(nv),
	                           ObservationMatrix_.findRow(pv));
}
void Node::setProbability(Matrix<float> m) { ProbabilityMatrix_ = m; }

void Node::setObservations(Matrix<int> m) { ObservationMatrix_ = m; }

void Node::setObservationBackup(Matrix<int> m) { ObservationMatrix_ = m; }

const std::string& Node::getName() const { return name_; }

const unsigned int& Node::getIndex() const { return index_; }

const unsigned int& Node::getID() const { return id_; }

bool Node::hasValue(std::string v) const
{
	if(ObservationMatrix_.findCol(v) > -1)
		return true;
	return false;
}

Matrix<float>& Node::getProbabilityMatrix() { return ProbabilityMatrix_; }

const Matrix<float>& Node::getProbabilityMatrix() const
{
	return ProbabilityMatrix_;
}

Matrix<int>& Node::getObservationMatrix() { return ObservationMatrix_; }

std::ostream& operator<<(std::ostream& os, const Node& n)
{
	os << n.name_ << " " << n.id_ << "\n" << n.ObservationMatrix_ << "\n"
	   << n.ProbabilityMatrix_ << std::endl;
	return os;
}

void Node::createBackup() { ObservationBackup_ = ObservationMatrix_; }

void Node::loadBackup() { ObservationMatrix_ = ObservationBackup_; }

const std::vector<unsigned int>& Node::getParents() const { return Parents_; }

void Node::setParents(std::vector<unsigned int> parents) { Parents_ = parents; }

void Node::cutParents() { Parents_.clear(); }

void Node::setUniqueValues(std::vector<int> uniqueValues)
{
	uniqueValues_ = uniqueValues;
}

const std::vector<int>& Node::getUniqueValues() const { return uniqueValues_; }

void Node::setUniqueValuesExcludingNA(std::vector<int> uniqueValues)
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

void Node::setObservationRow(int row) { observationRow_ = row; }

const int Node::getObservationRow() const { return observationRow_; }

void Node::setParentCombinations(int combinations)
{
	parentCombinations_ = combinations;
}

const int Node::getParentCombinations() const { return parentCombinations_; }

void Node::setValueNames(std::vector<std::string> valueNames)
{
	valueNames_ = valueNames;
}

void Node::addValueName(std::string name) { valueNames_.push_back(name); }

const std::vector<std::string>& Node::getValueNames() const
{
	return valueNames_;
}

void Node::setValueNamesProb(std::vector<std::string> valueNames)
{
	valueNamesProb_ = valueNames;
}

void Node::addValueNameProb(std::string names)
{
	valueNamesProb_.push_back(names);
}

const std::vector<std::string>& Node::getValueNamesProb() const
{
	return valueNamesProb_;
}

void Node::setParentValueNames(std::vector<std::string> valueNames)
{
	parentValueNames_ = valueNames;
}

void Node::addParentValueName(std::string names)
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
}

void Node::loadBackupDoIntervention()
{
	ProbabilityMatrix_ = ProbabilityMatrixBackup_;
	Parents_ = ParentsBackup_;
	ParentsBackup_.clear();
	ProbabilityMatrixBackup_ = Matrix<float>(0, 0, 0.0);
}

void Node::setProbabilityTo1(std::string value)
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

const int Node::getIndex(const std::string& value) const
{
	return ProbabilityMatrix_.findCol(value);
}

void Node::clearNameVectors()
{
	valueNamesProb_.clear();
	uniqueValues_.clear();
	valueNames_.clear();
	valueNamesProb_.clear();
	parentValueNames_.clear();
	uniqueValuesExcludingNA_.clear();
}
