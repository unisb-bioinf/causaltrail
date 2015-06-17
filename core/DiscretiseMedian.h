#ifndef DISCRETISEMEDIAN_H
#define DISCRETISEMEDIAN_H

#include "Discretisations.h"

class DiscretiseMedian : public Discretisations 
{
	public:
	DiscretiseMedian(unsigned int row, 
			const Matrix<std::string>& originalObservations, 
			Matrix<int>& discretisedObservations,
        		std::unordered_map<std::string,int>& observationsMap,
        		std::map<std::pair<int,int>, std::string>& observationsMapR);
	void apply() override;	
};
#endif
