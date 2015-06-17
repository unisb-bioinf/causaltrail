#ifndef DISCRETISEROUND_H
#define DISCRETISEROUND_H

#include "Discretisations.h"

class DiscretiseRound : public Discretisations 
{
	public:
	DiscretiseRound(unsigned int row, 
			const Matrix<std::string>& originalObservations, 
			Matrix<int>& discretisedObservations,
        		std::unordered_map<std::string,int>& observationsMap,
        		std::map<std::pair<int,int>, std::string>& observationsMapR);
	void apply() override;	
};
#endif
