#include "DataDistribution.h"

DataDistribution::DataDistribution(Network& network, Matrix<int>& observations)
    : network_(network),
      observations_(observations),
      observationsMap_(network.getObservationsMap()),
      observationsMapR_(network.getObservationsMapR())
{
}

int
DataDistribution::computeParentCombinations(std::vector<unsigned int> parents)
{
	int parentCombinations = 1;
	for(const auto& parent : parents) {
		parentCombinations *=
		    network_.getNode(parent).getNumberOfUniqueValuesExcludingNA();
	}
	return parentCombinations;
}

void DataDistribution::assignObservationsToNodes()
{
	for(auto& n : network_.getNodes()) {
		n.clearNameVectors();
		int row = observations_.findRow(n.getName());
		if (row == -1){
			throw std::invalid_argument(n.getName()+" not contained in data");
		}
		n.setObservationRow(row);
		n.setUniqueValues(
		    observations_.getUniqueRowValues(n.getObservationRow()));
		n.setUniqueValuesExcludingNA(
		    observations_.getUniqueRowValues(n.getObservationRow(), -1));
	}

	for(auto& n : network_.getNodes()) {
		n.setParentCombinations(computeParentCombinations(n.getParents()));
		assignValueNames(n);
		assignParentNames(n);
	}
}

void DataDistribution::assignValueNames(Node& n)
{
	for(const auto& value : n.getUniqueValues()) {
		const auto originalName =
		    observationsMapR_[std::make_pair(value, n.getObservationRow())];
		n.addValueName(originalName);
		if(value != -1) {
			n.addValueNameProb(originalName);
		}
	}
}

void DataDistribution::assignParentNames(Node& n)
{
	if(n.getNumberOfParents() > 1) {
		std::vector<std::vector<int>>
		uniqueValuesExcludingNA (network_.getNodes().size(),{-1});
		for(const auto& id : n.getParents()) {
			uniqueValuesExcludingNA[id] =
			    network_.getNode(id).getUniqueValuesExcludingNA();
		}

		auto comb = Combinations<int>(n.getParents(), uniqueValuesExcludingNA);
		comb.createCombinations(0);
		std::vector<std::vector<int>> tempParentNames = comb.getResult();
		std::vector<std::vector<int>> parentValues;
		for(auto& vec : tempParentNames) {
			std::string temp = "";
			std::vector<int> value;
			for(unsigned int key = 0; key < n.getNumberOfParents(); key++) {
				int parentRow =
				    network_.getNode(n.getParents()[key]).getObservationRow();
				temp = temp +
				       observationsMapR_[std::make_pair(vec[n.getParents()[key]], parentRow)] +
				       ",";
					value.push_back(vec[n.getParents()[key]]);
			}
			temp.erase(temp.end() - 1);
			n.addParentValueName(temp);
			parentValues.push_back(value);
		}
		n.setParentValues(parentValues);
	} else if(n.getNumberOfParents() == 1) {
		int parentRow = network_.getNode(n.getParents()[0]).getObservationRow();
		for(auto& v :
		    network_.getNode(n.getParents()[0]).getUniqueValuesExcludingNA()) {
			n.addParentValueName(
			    observationsMapR_[std::make_pair(v, parentRow)]);
		}
		std::vector<std::vector<int>> pValues;
		for (auto& value : network_.getNode(n.getParents()[0]).getUniqueValuesExcludingNA()){
			pValues.push_back({value});
		}
		n.setParentValues(pValues);

	} else {
		n.addParentValueName("1");
		std::vector<std::vector<int>> noParents{{}};
		n.setParentValues(noParents);
	}
}

int DataDistribution::getObservationColIndex(unsigned int sample, const Node& n)
{
	if(n.getNumberOfUniqueValues() != n.getNumberOfUniqueValuesExcludingNA()) {
		return observations_(sample, n.getObservationRow()) + 1;
	} else {
		return observations_(sample, n.getObservationRow());
	}
}

int DataDistribution::getObservationRowIndex(unsigned int sample, const Node& n,
                                             const Matrix<int>& obsMatrix)
{
	if(n.getParents().empty()) {
		return 0;
	}
	int index = 0;
	for(const auto& parentID : n.getParents()) {
		Node& pn = network_.getNode(parentID);
		int value = observations_(sample, pn.getObservationRow());
		if(value == -1) {
			return -1;
		}
		index += network_.computeFactor(n, parentID) * value;
	}
	return index;
}

void DataDistribution::countObservations(Matrix<int>& obsMatrix, const Node& n)
{
	for(unsigned int sample = 0; sample < observations_.getColCount();
	    sample++) {

		int column = getObservationColIndex(sample, n);

		int row = getObservationRowIndex(sample, n, obsMatrix);

		if(row != -1) {
			obsMatrix(column, row)++;
		}
	}
}

void DataDistribution::distributeObservations()
{
	// Generating matrices
	for(auto& n : network_.getNodes()) {
		// Generating suitable matrices
		Matrix<int> obsMatrix =
		    Matrix<int>(n.getValueNames(), n.getParentValueNames(), 0);
		Matrix<float> probMatrix =
		    Matrix<float>(n.getValueNamesProb(), n.getParentValueNames(), 0.0f);

		// Count observations
		countObservations(obsMatrix, n);

		// Store matrices
		n.setObservations(obsMatrix);
		n.setObservationBackup(obsMatrix);
		n.setProbability(probMatrix);
		n.clearDynProgMatrix();
		n.createBackup();
	}
}
