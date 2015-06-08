#include "SerializeDeserializeJson.h"
#include "Discretiser.h"
#include "DiscretisationFactory.h"
#include <algorithm>
#include "math.h"

Discretiser::Discretiser(const Matrix<std::string>& originalObservations,
                         Matrix<int>& obsMatrix, Network& network)
    : originalObservations_(originalObservations),
      observations_(obsMatrix),
      observationsMap_(network.getObservationsMap()),
      observationsMapR_(network.getObservationsMapR())
{
	adaptFormat();
	observations_.resize(originalObservations.getColCount(),
	                     originalObservations.getRowCount(), -1);
	observations_.setRowNames(originalObservations.getRowNames());
	observations_.setColNames(originalObservations.getColNames());
}

Discretiser::Discretiser(const Matrix<std::string>& originalObservations,
                         const std::string& filename, Matrix<int>& obsMatrix,
                         Network& network)
    : originalObservations_(originalObservations),
      observations_(obsMatrix),
      observationsMap_(network.getObservationsMap()),
      observationsMapR_(network.getObservationsMapR())
{
	adaptFormat();
	observations_.resize(originalObservations.getColCount(),
	                     originalObservations.getRowCount(), -1);
	observations_.setRowNames(originalObservations.getRowNames());
	observations_.setColNames(originalObservations.getColNames());
	discretise(filename);
}


int Discretiser::getEntry(unsigned int col, unsigned int row)
{
	int value = observations_(col, row);
	return value;
}

void Discretiser::discretise(const std::string& controlFile)
{
	SerializeDeserializeJson jsonTree(controlFile);
	//Create Factory
	DiscretisationFactory dF(jsonTree,
			originalObservations_,
                        observations_,
                        observationsMap_,
                        observationsMapR_);

	//Create Discretisations Objects
	for (unsigned int i = 0; i< originalObservations_.getRowCount(); i++){
		discretisations_.push_back(dF.create(originalObservations_.getRowNames()[i],i));	
	} 
	//createDiscretisationClasses(controlFile);
}
/*
void Discretiser::discretiseZ(unsigned int row)
{
	float expValue = 0.0;
	float expValue2 = 0.0;
	for(unsigned int col = 0; col < originalObservations_.getColCount();
	    col++) {
		float value = getNumber(col,row);
		expValue+=value;
		expValue2+=(value*value); 
	}
	expValue = expValue / originalObservations_.getColCount();
	expValue2 = expValue2 / originalObservations_.getColCount();
	float standardDeviation = sqrt(expValue2 - (expValue*expValue));
	for(unsigned int col = 0; col < originalObservations_.getColCount(); col++){
		float value = getNumber(col,row);
		float z = std::abs((value - expValue)/standardDeviation);
		if (z < 2.0f) {
			observations_.setData(0,col,row);		
		}
		else{
			observations_.setData(1,col,row);
		}
	}
}

void Discretiser::discretiseFloor(unsigned int row)
{
	for(unsigned int col = 0; col < originalObservations_.getColCount();
	    col++) {
		float value = getNumber(col, row);
		int dvalue = floor(value);
		observations_.setData(dvalue, col, row);
	}
}

void Discretiser::discretiseCeil(unsigned int row)
{
	for(unsigned int col = 0; col < originalObservations_.getColCount();
	    col++) {
	float value = getNumber(col, row);
		int dvalue = ceil(value);
		observations_.setData(dvalue, col, row);
	}
}

void Discretiser::discretiseRound(unsigned int row)
{
	for(unsigned int col = 0; col < originalObservations_.getColCount();
	    col++) {
		float value = getNumber(col, row);
		int dvalue = round(value);
		observations_.setData(dvalue, col, row);
	}
}

void Discretiser::discretiseByAMean(unsigned int row)
{
	float mean = 0;
	for(unsigned int col = 0; col < originalObservations_.getColCount();
	    col++) {
		float value = getNumber(col, row);
		mean += value;
	}
	mean = mean / originalObservations_.getColCount();
	for(unsigned int col = 0; col < originalObservations_.getColCount();
	    col++) {
		float value = getNumber(col, row);
		if(value > mean) {
			observations_.setData(1, col, row);
			createNameEntry(1, row);
		} else {
			observations_.setData(0, col, row);
			createNameEntry(0, row);
		}
	}
}

void Discretiser::discretiseByHMean(unsigned int row)
{
	float mean = 0;
	for(unsigned int col = 0; col < originalObservations_.getColCount();
	    col++) {
		float value = getNumber(col, row);
		mean += 1.0f / value;
	}
	mean = originalObservations_.getColCount() / mean;
	for(unsigned int col = 0; col < originalObservations_.getColCount();
	    col++) {
		float value = getNumber(col, row);
		if(value > mean) {
			observations_.setData(1, col, row);
			createNameEntry(1, row);
		} else {
			observations_.setData(0, col, row);
			createNameEntry(0, row);
		}
	}
}

void Discretiser::discretiseByMedian(unsigned int row)
{
	const std::vector<float>& templist = createSortedVector(row);
	float median;
	if(templist.size() % 2 != 0) {
		median = templist[ceil(templist.size() / 2)];
	} else {
		median = (templist[(templist.size() / 2) - 1] +
		          templist[templist.size() / 2]) /
		         2.0f;
	}
	for(unsigned int col = 0; col < originalObservations_.getColCount();
	    col++) {
		float value = getNumber(col, row);
		if(value > median) {
			observations_.setData(1, col, row);
			createNameEntry(1, row);
		} else {
			observations_.setData(0, col, row);
			createNameEntry(0, row);
		}
	}
}

void Discretiser::discretiseManually(unsigned int row, float threshold)
{
	for(unsigned int col = 0; col < originalObservations_.getColCount();
	    col++) {
		float value = getNumber(col, row);
		if(value > threshold) {
			observations_.setData(1, col, row);
			createNameEntry(1, row);
		} else {
			observations_.setData(0, col, row);
			createNameEntry(0, row);
		}
	}
}

void Discretiser::discretiseBracketMedians(unsigned int row,
                                           unsigned int number)
{
	const std::vector<float>& templist = createSortedVector(row);
	std::vector<float> borderValues;
	borderValues.reserve(number + 1);
	borderValues.push_back(templist[0]);
	// Calculat Borders
	for(unsigned int i = 1; i < number; i++) {
		borderValues.push_back(templist[templist.size() / number * i]);
	}
	borderValues.push_back(FLT_MAX);
	// Fill intervals
	for(unsigned int col = 0; col < templist.size(); col++) {
		float value = getNumber(col,row);
		for(unsigned int i = 1; i <= number; i++) {
			if((value >= borderValues[i - 1]) && (value <
			                                              borderValues[i])) {
				observations_.setData(i - 1, col, row);
				createNameEntry(i - 1, row);
				break;
			}
		}
	}
}

void Discretiser::discretisePearsonTukey(unsigned int row)
{
	const std::vector<float>& templist = createSortedVector(row);
	const std::vector<float> borderValues = {
	    // Calculate borders: Constants are defined by the method
	    FLT_MIN, templist[ceil(0.185 * templist.size())],
	    templist[ceil(0.815 * templist.size())], FLT_MAX};
	// Fill intervals
	for(unsigned int col = 0; col < templist.size(); col++) {
		for(int i = 1; i < 4; i++) {
			float value = getNumber(col, row);
			if(value >= borderValues[i - 1] && value < borderValues[i]) {
				observations_.setData(i - 1, col, row);
				createNameEntry(i - 1, row);
				break;
			}
		}
	}
}

void Discretiser::mapNamesToInt(unsigned int row)
{
	size_t index = 0;
	const auto& uniqueValues = originalObservations_.getUniqueRowValues(row);
	for(const auto& value : uniqueValues) {
		if((value != "NA")) {
			observationsMap_[value] = index;
			observationsMapR_[std::make_pair(index, row)] = value;
			index++;
		} else {
			observationsMap_[value] = -1;
			observationsMapR_[std::make_pair(-1, row)] = value;
		}
	}
	for(unsigned int col = 0; col < originalObservations_.getColCount();
	    col++) {
		observations_.setData(observationsMap_[originalObservations_(col, row)],
		                      col, row);
	}
}*/

void Discretiser::adaptFormat()
{
	static const std::string na1 = "na";
	static const std::string na2 = "-";
	static const std::string na3 = "/";

	static const std::string canonical_na("NA");

	for(unsigned int col = 0; col < originalObservations_.getColCount();
	    col++) {
		for(unsigned int row = 0; row < originalObservations_.getRowCount();
		    row++) {
			std::string& value = originalObservations_(col, row);
			if(value == na1 || value == na2 || value == na3) {
				value = canonical_na;
			}
		}
	}
}
