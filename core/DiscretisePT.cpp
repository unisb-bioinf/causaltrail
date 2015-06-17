#include "DiscretisePT.h"

DiscretisePT::DiscretisePT(unsigned int row,
                                const Matrix<std::string>& originalObservations,
                                Matrix<int>& discretisedObservations,
			        std::unordered_map<std::string,int>& observationsMap,
        			std::map<std::pair<int,int>, std::string>& observationsMapR)
	:Discretisations(row,
			originalObservations,
			discretisedObservations,
			observationsMap,
			observationsMapR)
{
}

void DiscretisePT::apply(){
	const std::vector<float>& templist = createSortedVector(row_);
	const std::vector<float> borderValues = {
	    // Calculate borders: Constants are defined by the method
	    FLT_MIN, templist[ceil(0.185 * templist.size())],
	    templist[ceil(0.815 * templist.size())], FLT_MAX};
	// Fill intervals
	for(unsigned int col = 0; col < templist.size(); col++) {
		for(int i = 1; i < 4; i++) {
			float value = getNumber(col, row_);
			if(value >= borderValues[i - 1] && value < borderValues[i]) {
				discretisedObservations_.setData(i - 1, col, row_);
				createNameEntry(i - 1, row_);
				break;
			}
		}
	}
}
