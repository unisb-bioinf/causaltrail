#include "Discretisations.h"

Discretisations::Discretisations(unsigned int row, 
				const Matrix<std::string>& originalObservations, 
				Matrix<int>& discretisedObservations)
	:row_(row),
	originalObservations_(originalObservations),
	discretisedObservations_(discretisedObservations)
{

}
