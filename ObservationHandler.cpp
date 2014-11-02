#include "ObservationHandler.h"

ObservationHandler::ObservationHandler(Network& network, Matrix<int>& observations, std::unordered_map<std::string,int>& observationsMap, std::map<std::pair<int,int>, std::string> & observationsMapR)
	:network_(network), observations_(observations), observationsMap_(observationsMap), observationsMapR_(observationsMapR)
	{
	assignObservationsToNodes();
	distributeObservations();
}

int ObservationHandler::computeParentCombinations(std::vector<unsigned int> parents){
	int parentCombinations=1;
	for (auto  parent: parents){
		parentCombinations*=network_.getNode(parent).getUniqueValuesExcludingNA().size();
	}
	return parentCombinations;
}

void ObservationHandler::assignObservationsToNodes(){
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

void ObservationHandler::assignValueNames(Node& n){
	for (auto value: n.getUniqueValues()){
		const auto originalName = observationsMapR_[std::make_pair(value,n.getObservationRow())];
		n.addValueName(originalName);
		if (value != -1) {
			n.addValueNameProb(originalName);
		}
	}
}

void ObservationHandler::assignParentNames(Node& n){
	if (n.getParents().size() > 1){
		std::unordered_map<int,std::vector<int>> uniqueValuesExcludingNA;
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

void ObservationHandler::countObservations(Matrix<int>& obsMatrix, Node& n){
	for (unsigned int sample = 0; sample<observations_.getColCount();sample++){
			std::string rowName="";
			std::string colName=observationsMapR_[std::make_pair(observations_(sample,n.getObservationRow()),n.getObservationRow())];
		for (auto parentID : n.getParents()){
			int row=network_.getNode(parentID).getObservationRow();	
			rowName=rowName+observationsMapR_[std::make_pair(observations_(sample,row),row)]+",";
		}
		if (rowName==""){
			rowName="1";
		}
		if ((rowName.find("NA")==std::string::npos) and (rowName.find("na")==std::string::npos) and (rowName.find("-")==std::string::npos)){
			if (rowName != "1"){
			rowName.erase(rowName.end()-1);
			}
		obsMatrix.setData(obsMatrix.getValueByNames(colName,rowName)+1,obsMatrix.findCol(colName),obsMatrix.findRow(rowName));
		}
	}
}

/**distributeObservations
 *
 * @return void
 *
 * Assigns the discretised observations to the nodes in the network according to their names and the network structure
 */
void ObservationHandler::distributeObservations(){
	//Generating matrices
	for (auto& n:network_.getNodes()){
		//Generating suitable matrices
		Matrix<int> obsMatrix = Matrix<int>(n.getUniqueValues().size(),n.getParentCombinations(),0);
		obsMatrix.setColNames(n.getValueNames());
		obsMatrix.setRowNames(n.getParentValueNames());
		Matrix<float> probMatrix = Matrix<float>(n.getUniqueValuesExcludingNA().size(),n.getParentCombinations(),0.0);
		probMatrix.setColNames(n.getValueNamesProb());	
		probMatrix.setRowNames(n.getParentValueNames());
		//Count observations
		countObservations(obsMatrix,n);
		//Store matrices
		n.setObservations(obsMatrix);
		n.setObservationBackup(obsMatrix);
		n.setProbability(probMatrix);
		n.createBackup();
	}
}
