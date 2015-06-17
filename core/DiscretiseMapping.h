#ifndef DISCRETISEMAPPING_H
#define DISCRETISEMAPPING_H

#include "Discretisations.h"

class DiscretiseMapping : public Discretisations 
{
	public:
	DiscretiseMapping(unsigned int row, 
			const Matrix<std::string>& originalObservations, 
			Matrix<int>& discretisedObservations,
        		std::unordered_map<std::string,int>& observationsMap,
        		std::map<std::pair<int,int>, std::string>& observationsMapR);
	void apply() override;	
};
#endif
