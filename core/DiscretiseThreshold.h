#ifndef DISCRETISETHRESHOLD_H
#define DISCRETISETHRESHOLD_H

#include "Discretisations.h"

class DiscretiseThreshold : public Discretisations 
{
	public:
	DiscretiseThreshold(unsigned int row, 
			const Matrix<std::string>& originalObservations, 
			Matrix<int>& discretisedObservations,
        		std::unordered_map<std::string,int>& observationsMap,
        		std::map<std::pair<int,int>, std::string>& observationsMapR,
			float threshold);
	void apply() override;	

	private:
	float threshold_;
};
#endif
