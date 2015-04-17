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
				if(!parentsKnown && (usedValues.find(v) ==
				                         usedValues.end())) {
					val_.insert(val_.end(), v.begin(), v.end());
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
					if(useRow && (usedValues.find(v) == usedValues.end())) {
						val_.insert(val_.end(), v.begin(), v.end());
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
			if(!parentsKnown && (usedValues.find(v) == usedValues.end())) {
				val_.insert(val_.end(), v.begin(), v.end());
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
				if(useRow && (usedValues.find(v) == usedValues.end())) {
					val_.insert(val_.end(), v.begin(), v.end());
					addProbability(p(values[n.getID()], row));
					length_++;
					usedValues.insert(v);
				}
			}
		}
	}
}

Factor::Factor(unsigned int length, std::vector<unsigned int> ids)
	: val_(length*ids.size()), probabilities_(length), length_(length)
{
	nodeIDs_ = ids;
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

void Factor::normalize(){
	if (probabilities_.size() > 1){
		float probSum = 0.0f;
		for (auto& v : probabilities_){
			probSum+=v;
		}
		if (probSum > 0.0f){
			for (auto& v : probabilities_){
				v = v / probSum;
			}
		}
	}
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
	Factor newFactor(newFactorLength, unionIDs);
	int counter = 0;
	for (int i = 0; i< length_; i++){
		for (int j = 0; j < factor.length_; j++){
			bool valid = true;
			for(auto& id : commonIDs) {
				if(val_[getIndex(id)+i*nodeIDs_.size()] != factor.val_[factor.getIndex(id)+j*factor.nodeIDs_.size()]) {
					valid = false;
					break;
				}
			}
			if(valid) {
				for (unsigned int h = 0; h < nodeIDs_.size(); h++){
					newFactor.val_[h+counter*unionIDs.size()]=val_[h+i*nodeIDs_.size()];
				}
				int h=nodeIDs_.size();
				for (auto id : uniqueIDs){
					newFactor.val_[h+counter*unionIDs.size()]=factor.val_[factor.getIndex(id)+j*factor.nodeIDs_.size()];
					h++;
				}	
				newFactor.setProbability(getProbability(i) *
				                         factor.getProbability(j),counter);
				counter++;
			}
		}
	}
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
	unsigned int newFactorLength = length_;
	unsigned int totalCombinations = jump;
	if(values[id] == -1) {
		jumptimes =
		    network_.getNode(id).getNumberOfUniqueValuesExcludingNA() - 1;
		totalCombinations*=network_.getNode(id).getNumberOfUniqueValuesExcludingNA();
		newFactorLength = length_/(jumptimes+1);
	}
	
	std::vector<unsigned int> newIDs = nodeIDs_;
	newIDs.erase(newIDs.begin() + index);
	Factor newFactor(newFactorLength, newIDs);
	for (unsigned int i = 0; i< newFactorLength; i++){
		float prob = 0.0;
		unsigned int old = (i % jump) + totalCombinations*div(i,jump).quot;
		for (unsigned int j = 0; j<=jumptimes; j++){
			prob+=probabilities_[old+j*jump];
		}
		for (unsigned int c = 0; c < nodeIDs_.size(); c++){
				unsigned int d = c;
				if (c != index){
					if (c > index){
						d = c - 1;
					}
					newFactor.val_[d+i*newIDs.size()]=val_[c+old*nodeIDs_.size()];
				}
			}
			newFactor.setProbability(prob,i);	
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

void Factor::addProbability(float prob) { probabilities_.push_back(prob); }

void Factor::setProbability(float prob, unsigned int index)
{
	probabilities_[index] = prob;
}

float Factor::getProbability(unsigned int index) const
{
	return probabilities_[index];
}

float Factor::getProbability(const std::vector<int>& values) const
{
	for(int i = 0; i< length_; i++){
		bool flag = true;
		if (nodeIDs_.size() > 0){
			for (unsigned int j = 0; j < nodeIDs_.size(); j++){
				if (val_[j+i*nodeIDs_.size()] != values[nodeIDs_[j]]){
					flag = false;
				}
			}
			if (flag){
				return probabilities_[i];
				}
			}
		}
	return 1.0f;
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
		for (unsigned int j = 0; j < f.nodeIDs_.size(); j++){
			os << f.val_[j+i*f.nodeIDs_.size()]<< " ";
		}
		os << f.probabilities_[i] << std::endl;
		;
	}
	return os;
}

