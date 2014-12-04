#include "Factor.h"
#include "cmath"
#include "algorithm"

Factor::Factor(const Node& n, const std::vector<int>& values)
{
	std::vector<unsigned int> nodeIDs = {n.getID()};
	nodeIDs.insert(nodeIDs.end(), n.getParents().begin(), n.getParents().end());
	nodeIDs_ = nodeIDs;
	length_ = 0;
	Matrix<float> p = n.getProbabilityMatrix();
	bool parentsKnown = false;
	std::vector<unsigned int> knownParents;
	std::set<std::vector<int>> usedValues;
	for(auto& id : n.getParents()) {
		if(values[id] != -1) {
			parentsKnown = true;
			knownParents.push_back(id);
		}
	}
	if(values[n.getID()] == -1) {
		for(unsigned int col = 0; col < p.getColCount(); col++) {
			for(unsigned int row = 0; row < p.getRowCount(); row++) {
				std::vector<int> v{n.getUniqueValuesExcludingNA()[col]};
				if(n.getNumberOfParents() != 0) {
					v.insert(v.end(), n.getParentValues()[row].begin(),
					         n.getParentValues()[row].end());
				}
				if((not parentsKnown)and(usedValues.find(v) ==
				                         usedValues.end())) {
					addValue(v);
					addProbability(p(col, row));
					length_++;
					usedValues.insert(v);
				} else {
					bool useRow = true;
					for(auto& id : knownParents) {
						if(v[getIndex(id)] != values[id]) {
							useRow = false;
						}
					}
					if((useRow)and(usedValues.find(v) == usedValues.end())) {
						addValue(v);
						addProbability(p(col, row));
						length_++;
						usedValues.insert(v);
					}
				}
			}
		}
	} else {
		for(unsigned int row = 0; row < p.getRowCount(); row++) {
			std::vector<int> v{values[n.getID()]};
			if(n.getNumberOfParents() != 0) {
				v.insert(v.end(), n.getParentValues()[row].begin(),
				         n.getParentValues()[row].end());
			}
			if((not parentsKnown)and(usedValues.find(v) == usedValues.end())) {
				addValue(v);
				addProbability(p(values[n.getID()], row));
				length_++;
				usedValues.insert(v);
			} else {
				bool useRow = true;
				for(auto& id : knownParents) {
					if(v[getIndex(id)] != values[id]) {
						useRow = false;
					}
				}
				if((useRow)and(usedValues.find(v) == usedValues.end())) {
					addValue(v);
					addProbability(p(values[n.getID()], row));
					length_++;
					usedValues.insert(v);
				}
			}
		}
	}
}

Factor::Factor(unsigned int length, std::vector<unsigned int> ids)
//	: values_(length * ids.size()), probabilities_(length)
{
	nodeIDs_ = ids;
	length_ = length;
	values_.resize(length, {});
	probabilities_.resize(length, 0.0f);
}

std::vector<unsigned int> Factor::getCommonIDs(const Factor& factor) const
{
	std::vector<unsigned int> intersection;
	intersection.reserve(std::min(nodeIDs_.size(), factor.getIDs().size()));

	std::vector<unsigned int> thisIDs = nodeIDs_;
	std::vector<unsigned int> otherIDs = factor.getIDs();
	std::sort(thisIDs.begin(), thisIDs.end());
	std::sort(otherIDs.begin(), otherIDs.end());

	std::set_intersection(thisIDs.begin(), thisIDs.end(), otherIDs.begin(),
	                      otherIDs.end(), std::back_inserter(intersection));
	return intersection;
}

std::vector<unsigned int>
Factor::getUnionOfIDs(const std::vector<unsigned int>& commonIDs,
                      const Factor& factor) const
{
	std::vector<unsigned int> uni = nodeIDs_;
	for(auto& id : factor.getIDs()) {
		if(std::find(commonIDs.begin(), commonIDs.end(), id) ==
		   commonIDs.end()) {
			uni.push_back(id);
		}
	}
	return uni;
}

Factor Factor::product(Factor& factor, const Network& network_, const std::vector<int>& values)
{
	std::vector<unsigned int> commonIDs = getCommonIDs(factor);
	std::vector<unsigned int> unionIDs = getUnionOfIDs(commonIDs, factor);
	std::vector<unsigned int> uniqueIDs(unionIDs.begin() + nodeIDs_.size(),
	                                    unionIDs.end());
	int newFactorLength = 1;
	for (auto& id : unionIDs){
		if (values[id] == -1){
			newFactorLength*=network_.getNode(id).getNumberOfUniqueValuesExcludingNA();
		}
	}
	std::cout<<newFactorLength<<std::endl;
	Factor newFactor(0, unionIDs);
	int i = 0;
	for(auto& valueAssignment1 : values_) {
		int j = 0;
		for(auto& valueAssignment2 : factor.getAllValues()) {
			// Assert that all common nodes are identical
			bool valid = true;
			for(auto& id : commonIDs) {
				if(valueAssignment1[getIndex(id)] !=
				   valueAssignment2[factor.getIndex(id)]) {
					valid = false;
					break;
				}
			}
			if(valid) {
				std::vector<int> assignment(valueAssignment1.size() + uniqueIDs.size());
				std::copy(valueAssignment1.begin(), valueAssignment1.end(), assignment.begin());
				auto it = assignment.begin() + valueAssignment1.size();
				for(auto id : uniqueIDs) {
					*it++ = valueAssignment2[factor.getIndex(id)];
				}
				newFactor.addValue(assignment);
				newFactor.addProbability(getProbability(i) *
				                         factor.getProbability(j));
				newFactor.length_ += 1;
			}
			j++;
		}
		i++;
	}
	std::cout<<"RealLength: "<<newFactor.length_<<std::endl;;
	return newFactor;
}

Factor Factor::sumOut(unsigned int id, Network& network_,
                      const std::vector<int>& values)
{
	unsigned int index = getIndex(id);
	unsigned int jump = 1;
	for(unsigned int i = index + 1; i < nodeIDs_.size(); i++) {
		if(values[nodeIDs_[i]] == -1) {
			jump *= network_.getNode(nodeIDs_[i])
			            .getNumberOfUniqueValuesExcludingNA();
		}
	}
	unsigned int jumptimes = 0;
	if(values[id] == -1) {
		jumptimes =
		    network_.getNode(id).getNumberOfUniqueValuesExcludingNA() - 1;
	}
	nodeIDs_.erase(nodeIDs_.begin() + index);
	Factor newFactor(0, nodeIDs_);
	int firstOb = values_[0][index];
	for(int i = 0; i < length_; i++) {
		if(values_[i][index] == firstOb) {
			float prob = probabilities_[i];
			for(unsigned int j = 1; j <= jumptimes; j++) {
				prob += probabilities_[i + j * jump];
			}
			values_[i].erase(values_[i].begin() + index);
			newFactor.addValue(values_[i]);
			newFactor.addProbability(prob);
			newFactor.length_ += 1;
		}
	}

	return newFactor;
}

unsigned int Factor::getIndex(unsigned int id) const
{
	for(unsigned int i = 0; i < nodeIDs_.size(); i++) {
		if(nodeIDs_[i] == id) {
			return i;
		}
	}
	throw std::invalid_argument("In Factor::getIndex, element with the given "
	                            "id is not listed as a parent of the node "
	                            "represented by this factor");
}

const std::vector<unsigned int>& Factor::getIDs() const { return nodeIDs_; }
const std::vector<std::vector<int>>& Factor::getAllValues() const
{
	return values_;
}

const std::vector<int>& Factor::getValues(unsigned int index) const
{
	return values_[index];
}

const int& Factor::getValue(unsigned int row, unsigned int index) const
{
	return values_[row][index];
}
	
void Factor::setValue(const std::vector<int>& value, unsigned int index){
	values_[index]=value;
}

void Factor::addValue(const std::vector<int>& value)
{
	values_.push_back(value);
}

void Factor::addProbability(float prob) { probabilities_.push_back(prob); }

void Factor::setProbability(float prob, unsigned int index)
{
	probabilities_[index] = prob;
}

float Factor::getProbability(unsigned int index) const
{
	return probabilities_[index];
}

std::ostream& operator<<(std::ostream& os, const Factor& f)
{
	os << "IDs:"
	   << "\n";
	for(auto& id : f.nodeIDs_) {
		os << id << " ";
	}
	os << "\n"
	   << "Table:" << std::endl;
	for(int i = 0; i < f.length_; i++) {
		for(auto& id : f.values_[i]) {
			os << id << " ";
		}
		os << f.probabilities_[i] << std::endl;
		;
	}
	return os;
}

