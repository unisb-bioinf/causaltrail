#ifndef DISCRETISEFLOOR_H
#define DISCRETISEFLOOR_H

#include "Discretisations.h"

class DiscretiseFloor : public Discretisations 
{
	public:
	DiscretiseFloor(unsigned int row, 
			const Matrix<std::string>& originalObservations, 
			Matrix<int>& discretisedObservations,
        		std::unordered_map<std::string,int>& observationsMap,
        		std::map<std::pair<int,int>, std::string>& observationsMapR);
	void apply() override;	
};
#endif
