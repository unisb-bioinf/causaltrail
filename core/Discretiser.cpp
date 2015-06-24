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

void Discretiser::setJsonTree(const DiscretisationSettings& jsonTree)
{
	jsonTree_ = jsonTree;
}

void Discretiser::discretise(const std::string& controlFile)
{
	jsonTree_ = DiscretisationSettings(controlFile);

	discretise();
}

void Discretiser::discretise()
{

	DiscretisationFactory dF(jsonTree_);

	// Create Discretisations Objects
	for(unsigned int i = 0; i < observations_.getRowCount(); i++) {
		discretisations_.push_back(dF.create(observations_.getRowNames()[i]));
	}

	Discretisations::Data data(originalObservations_, observations_,
	                           network_.getObservationsMap(),
	                           network_.getObservationsMapR());

	// Apply Discretisation;
	unsigned int row = 0;
	for(auto& ptr : discretisations_) {
		ptr->apply(row, data);
		++row;
	}
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
