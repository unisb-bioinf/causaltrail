#ifndef DISCRETISEARITHMETICMEAN_H
#define DISCRETISEARITHMETICMEAN_H

#include "Discretisations.h"

class DiscretiseArithmeticMean : public Discretisations 
{
	public:
	DiscretiseArithmeticMean(unsigned int row, 
			const Matrix<std::string>& originalObservations, 
			Matrix<int>& discretisedObservations,
        		std::unordered_map<std::string,int>& observationsMap,
        		std::map<std::pair<int,int>, std::string>& observationsMapR);
	void apply() override;	
};
#endif
