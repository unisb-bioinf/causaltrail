#ifndef DISCRETISEFLOOR_H
#define DISCRETISEFLOOR_H

#include "Discretisations.h"

class DiscretiseFloor : public Discretisations 
{
	public:
	DiscretiseFloor(unsigned int row, const Matrix<std::string>& originalObservations, Matrix<int>& discretisedObservations);
	void apply();	
};
#endif
