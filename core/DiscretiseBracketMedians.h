#ifndef DISCRETISEBRACKETMEDIANS_H
#define DISCRETISEBRACKETMEDIANS_H

#include "Discretisations.h"

class DiscretiseBracketMedians : public Discretisations 
{
	public:
	DiscretiseBracketMedians(unsigned int row, 
			const Matrix<std::string>& originalObservations, 
			Matrix<int>& discretisedObservations,
        		std::unordered_map<std::string,int>& observationsMap,
        		std::map<std::pair<int,int>, std::string>& observationsMapR,
			int buckets);
	void apply() override;	

	private:
	int buckets_;
};
#endif
