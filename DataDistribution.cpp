#include "DataDistribution.h"

/*
 *
 *
 *
 *
 *
 *
 */
DataDistribution::DataDistribution(Network& network, Matrix<int>& observations)
	:network_(network), observations_(observations), observationsMap_(network.getObservationsMap()), observationsMapR_(network.getObservationsMapR())
	{
	assignObservationsToNodes();
	distributeObservations();
}

/*
 *
 *
 *
 *
 *
 *
 */
int DataDistribution::computeParentCombinations(std::vector<unsigned int> parents){
	int parentCombinations=1;
	for (auto  parent: parents){
		parentCombinations*=network_.getNode(parent).getUniqueValuesExcludingNA().size();
	}
	return parentCombinations;
}

/*
 *
 *
 *
 *
 *
 *
 */
void DataDistribution::assignObservationsToNodes(){
	for (auto& n: network_.getNodes()){
		n.setObservationRow(observations_.findRow(n.getName()));
		n.setUniqueValues(observations_.getUniqueRowValues(n.getObservationRow()));
		n.setUniqueValuesExcludingNA(observations_.getUniqueRowValues(n.getObservationRow(),-1));	
	}

	for (auto& n:network_.getNodes()){	
		n.setParentCombinations(computeParentCombinations(n.getParents()));
		assignValueNames(n);
		assignParentNames(n);
	}
}

/*
 *
 *
 *
 *
 *
 *
 */
void DataDistribution::assignValueNames(Node& n){
	for (auto value: n.getUniqueValues()){
		const auto originalName = observationsMapR_[std::make_pair(value,n.getObservationRow())];
		n.addValueName(originalName);
		if (value != -1) {
			n.addValueNameProb(originalName);
		}
	}
}

/*
 *
 *
 *
 *
 *
 *
 */
void DataDistribution::assignParentNames(Node& n){
	if (n.getParents().size() > 1){
		std::unordered_map<unsigned int,std::vector<int>> uniqueValuesExcludingNA;
		for (auto id : n.getParents()){
			uniqueValuesExcludingNA[id]=network_.getNode(id).getUniqueValuesExcludingNA();
		}

		auto comb = Combinations<int>(n.getParents(), uniqueValuesExcludingNA);
		comb.createCombinations(0);
		std::vector<std::vector<int>> tempParentNames=comb.getResult();

		for (auto vec : tempParentNames){
			std::string temp="";
			for (unsigned int key=0; key<n.getParents().size();key++){
				int parentRow=network_.getNode(n.getParents()[key]).getObservationRow();
				temp=temp+observationsMapR_[std::make_pair(vec[key],parentRow)]+",";
			}
			temp.erase(temp.end()-1);
			n.addParentValueName(temp);
		}
	}

	else if (n.getParents().size()==1){
		int parentRow=network_.getNode(n.getParents()[0]).getObservationRow();
		for (auto v : network_.getNode(n.getParents()[0]).getUniqueValuesExcludingNA()){
			n.addParentValueName(observationsMapR_[std::make_pair(v,parentRow)]); 
		}

	} else {
		n.addParentValueName("1");
	}
}

/*
 *
 *
 *
 *
 *
 *
 */
unsigned int DataDistribution::getObservationColIndex(unsigned int sample, Node& n, Matrix<int>& obsMatrix){
	const std::string& colName = observationsMapR_[std::make_pair(
			observations_(sample, n.getObservationRow()),
			n.getObservationRow()
		)];
	return obsMatrix.findCol(colName);

}

/*
 *
 *
 *
 *
 *
 *
 */
int DataDistribution::getObservationRowIndex(unsigned int sample, Node& n, Matrix<int>& obsMatrix){
	if (n.getParents().empty()){
		return 0;
	}
	std::string rowName="";
	for (auto parentID : n.getParents()){
			int row=network_.getNode(parentID).getObservationRow();	
			rowName=rowName+observationsMapR_[std::make_pair(observations_(sample,row),row)]+",";
	}
	if ((rowName.find("NA")==std::string::npos)){
		rowName.erase(rowName.end()-1);
		return obsMatrix.findRow(rowName);
	}
	return -1;
}

/*
 *
 *
 *
 *
 *
 *
 */
void DataDistribution::countObservations(Matrix<int>& obsMatrix, Node& n){
	for (unsigned int sample = 0; sample<observations_.getColCount();sample++){
		unsigned int column = getObservationColIndex(sample,n, obsMatrix);
		int row = getObservationRowIndex(sample,n,obsMatrix);
		if (row != -1){
			obsMatrix(column,row)++;		
		}
	}
}

/**distributeObservations
 *
 * @return void
 *
 * Assigns the discretised observations to the nodes in the network according to their names and the network structure
 */
void DataDistribution::distributeObservations(){
	//Generating matrices
	for (auto& n:network_.getNodes()){
		//Generating suitable matrices
		Matrix<int> obsMatrix = Matrix<int>(n.getUniqueValues().size(),n.getParentCombinations(),0, n.getValueNames(), n.getParentValueNames());
		Matrix<float> probMatrix = Matrix<float>(n.getUniqueValuesExcludingNA().size(),n.getParentCombinations(),0.0, n.getValueNamesProb(), n.getParentValueNames());

		//Count observations
		countObservations(obsMatrix,n);

		//Store matrices
		n.setObservations(obsMatrix);
		n.setObservationBackup(obsMatrix);
		n.setProbability(probMatrix);
		n.createBackup();
	}
}
