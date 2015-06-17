#include "DiscretiseHarmonicMean.h"

DiscretiseHarmonicMean::DiscretiseHarmonicMean(unsigned int row,
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

void DiscretiseHarmonicMean::apply(){
	float mean = 0;
	for(unsigned int col = 0; col < originalObservations_.getColCount();
	    col++) {
		float value = getNumber(col, row_);
		mean += 1.0f / value;
	}
	mean = originalObservations_.getColCount() / mean;
	for(unsigned int col = 0; col < originalObservations_.getColCount();
	    col++) {
		float value = getNumber(col, row_);
		if(value > mean) {
			discretisedObservations_.setData(1, col, row_);
			createNameEntry(1, row_);
		} else {
			discretisedObservations_.setData(0, col, row_);
			createNameEntry(0, row_);
		}
	}
}
