#include "DiscretiseRound.h"

DiscretiseRound::DiscretiseRound(unsigned int row,
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

void DiscretiseRound::apply(){
	for(unsigned int col = 0; col < originalObservations_.getColCount();
            col++) {
                float value = getNumber(col, row_);
                int dvalue = round(value);
                discretisedObservations_.setData(dvalue, col, row_);
	}
	convertToDenseNumbers(row_);
}
