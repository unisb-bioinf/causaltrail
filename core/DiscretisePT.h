#ifndef DISCRETISEPT_H
#define DISCRETISEPT_H

#include "Discretisations.h"

class DiscretisePT : public Discretisations 
{
	public:
	DiscretisePT(unsigned int row, 
			const Matrix<std::string>& originalObservations, 
			Matrix<int>& discretisedObservations,
        		std::unordered_map<std::string,int>& observationsMap,
        		std::map<std::pair<int,int>, std::string>& observationsMapR);
	void apply() override;	
};
#endif
