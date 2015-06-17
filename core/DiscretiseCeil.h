#ifndef DISCRETISECEIL_H
#define DISCRETISECEIL_H

#include "Discretisations.h"

class DiscretiseCeil : public Discretisations 
{
	public:
	DiscretiseCeil(unsigned int row, 
			const Matrix<std::string>& originalObservations, 
			Matrix<int>& discretisedObservations,
        		std::unordered_map<std::string,int>& observationsMap,
        		std::map<std::pair<int,int>, std::string>& observationsMapR);
	void apply() override;	
};
#endif
