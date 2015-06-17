#include "DiscretiseZScore.h"

DiscretiseZScore::DiscretiseZScore(unsigned int row,
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

void DiscretiseZScore::apply(){
	float expValue = 0.0;
	float expValue2 = 0.0;
	for(unsigned int col = 0; col < originalObservations_.getColCount();
	    col++) {
		float value = getNumber(col,row_);
		expValue+=value;
		expValue2+=(value*value); 
	}
	expValue = expValue / originalObservations_.getColCount();
	expValue2 = expValue2 / originalObservations_.getColCount();
	float standardDeviation = sqrt(expValue2 - (expValue*expValue));
	for(unsigned int col = 0; col < originalObservations_.getColCount(); col++){
		float value = getNumber(col,row_);
		float z = std::abs((value - expValue)/standardDeviation);
		if (z < 2.0f) {
			discretisedObservations_.setData(0,col,row_);		
		}
		else{
			discretisedObservations_.setData(1,col,row_);
		}
	}
}
