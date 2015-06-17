#include "DiscretiseThreshold.h"

DiscretiseThreshold::DiscretiseThreshold(unsigned int row,
                                const Matrix<std::string>& originalObservations,
                                Matrix<int>& discretisedObservations,
			        std::unordered_map<std::string,int>& observationsMap,
        			std::map<std::pair<int,int>, std::string>& observationsMapR,
				float threshold)
	:Discretisations(row,
			originalObservations,
			discretisedObservations,
			observationsMap,
			observationsMapR),threshold_(threshold)
{
}

void DiscretiseThreshold::apply(){
	for(unsigned int col = 0; col < originalObservations_.getColCount();
	    col++) {
		float value = getNumber(col, row_);
		if(value > threshold_) {
			discretisedObservations_.setData(1, col, row_);
			createNameEntry(1, row_);
		} else {
			discretisedObservations_.setData(0, col, row_);
			createNameEntry(0, row_);
		}
	}
}
