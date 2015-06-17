#ifndef DISCRETISEZSCORE_H
#define DISCRETISEZSCORE_H

#include "Discretisations.h"

class DiscretiseZScore : public Discretisations 
{
	public:
	DiscretiseZScore(unsigned int row, 
			const Matrix<std::string>& originalObservations, 
			Matrix<int>& discretisedObservations,
        		std::unordered_map<std::string,int>& observationsMap,
        		std::map<std::pair<int,int>, std::string>& observationsMapR);
	void apply() override;	
};
#endif
