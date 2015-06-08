#include "Discretisations.h"

Discretisations::Discretisations(unsigned int row, 
				const Matrix<std::string>& originalObservations, 
				Matrix<int>& discretisedObservations,
			        std::unordered_map<std::string,int>& observationsMap,
        			std::map<std::pair<int,int>, std::string>& observationsMapR)
	:row_(row),
	originalObservations_(originalObservations),
	discretisedObservations_(discretisedObservations),
	observationsMap_(observationsMap),
	observationsMapR_(observationsMapR)
{

}

float Discretisations::getNumber(unsigned int col, unsigned int row)
{
	std::stringstream ss;
	std::string temp;
	float value;
	temp = originalObservations_(col, row);
	if(temp == "NA")
		return -1;
	ss << temp;
	ss >> value;
	return value;
}

void Discretisations::createNameEntry(int value, unsigned int row)
{
	std::stringstream ss;
	std::string ssvalue;
	ss << value;
	ss >> ssvalue;
	observationsMap_[ssvalue] = value;
	observationsMapR_[std::make_pair(value, row)] = ssvalue;
}

std::vector<float> Discretisations::createSortedVector(unsigned int row)
{
	std::vector<float> templist;
	for(unsigned int col = 0; col < originalObservations_.getColCount();
	    col++) {
		templist.push_back(getNumber(col, row));
	}
	std::sort(templist.begin(), templist.end());
	return templist;
}

void Discretisations::convertToDenseNumbers(unsigned int row)
{
	const std::vector<int>& obs = discretisedObservations_.getUniqueRowValues(row);
	for(unsigned int key = 0; key < obs.size(); key++) {
		for(unsigned int col = 0; col < discretisedObservations_.getColCount(); col++) {
			if(obs[key] == discretisedObservations_(col, row)) {
				discretisedObservations_.setData(key, col, row);
				createNameEntry(key, row);
			}
		}
	}
}


