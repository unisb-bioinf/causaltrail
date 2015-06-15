#include "Discretiser.h"
#include "DiscretisationFactory.h"
#include <algorithm>
#include "math.h"

Discretiser::Discretiser(const Matrix<std::string>& originalObservations,
                         Matrix<int>& obsMatrix, Network& network)
    : originalObservations_(originalObservations),
      observations_(obsMatrix),
      network_(network)
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
      network_(network)
{
	adaptFormat();
	observations_.resize(originalObservations.getColCount(),
	                     originalObservations.getRowCount(), -1);
	observations_.setRowNames(originalObservations.getRowNames());
	observations_.setColNames(originalObservations.getColNames());
	discretise(filename);
}


void Discretiser::discretise(const std::string& controlFile)
{
	jsonTree_= SerializeDeserializeJson(controlFile);
	//Create Factory
	DiscretisationFactory dF(jsonTree_,
			originalObservations_,
                        observations_,
                        network_.getObservationsMap(),
                        network_.getObservationsMapR());

	//Create Discretisations Objects
	for (int i = 0; i < observations_.getRowCount(); i++){
		discretisations_.push_back(dF.create(observations_.getRowNames()[i],i));	
	}
 
	//Apply Discretisation;
	for (std::unique_ptr<Discretisations>& ptr : discretisations_){
		ptr->apply();
	}
}

void Discretiser::discretise(){
	//Apply Discretisation;
//	for (std::unique_ptr<Discretisations>& ptr : discretisations_){
//		ptr->apply();
//	}
}

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
