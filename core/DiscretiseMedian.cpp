#include "DiscretiseMedian.h"

DiscretiseMedian::DiscretiseMedian(unsigned int row,
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

void DiscretiseMedian::apply(){
	const std::vector<float>& templist = createSortedVector(row_);
	float median;
	if(templist.size() % 2 != 0) {
		median = templist[ceil(templist.size() / 2)];
	} else {
		median = (templist[(templist.size() / 2) - 1] +
		          templist[templist.size() / 2]) /
		         2.0f;
	}
	for(unsigned int col = 0; col < originalObservations_.getColCount();
	    col++) {
		float value = getNumber(col, row_);
		if(value > median) {
			discretisedObservations_.setData(1, col, row_);
			createNameEntry(1, row_);
		} else {
			discretisedObservations_.setData(0, col, row_);
			createNameEntry(0, row_);
		}
	}
}
