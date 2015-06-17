#ifndef DISCRETISEHARMONICMEAN_H
#define DISCRETISEHARMONICMEAN_H

#include "Discretisations.h"

class DiscretiseHarmonicMean : public Discretisations 
{
	public:
	DiscretiseHarmonicMean(unsigned int row, 
			const Matrix<std::string>& originalObservations, 
			Matrix<int>& discretisedObservations,
        		std::unordered_map<std::string,int>& observationsMap,
        		std::map<std::pair<int,int>, std::string>& observationsMapR);
	void apply() override;	
};
#endif
