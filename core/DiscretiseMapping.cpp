#include "DiscretiseMapping.h"

DiscretiseMapping::DiscretiseMapping(unsigned int row,
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

void DiscretiseMapping::apply(){
	size_t index = 0;
	const auto& uniqueValues = originalObservations_.getUniqueRowValues(row_);
	for(const auto& value : uniqueValues) {
		if((value != "NA")) {
			observationsMap_[value] = index;
			observationsMapR_[std::make_pair(index, row_)] = value;
			index++;
		} else {
			observationsMap_[value] = -1;
			observationsMapR_[std::make_pair(-1, row_)] = value;
		}
	}
	for(unsigned int col = 0; col < originalObservations_.getColCount();
	    col++) {
		discretisedObservations_.setData(observationsMap_[originalObservations_(col, row_)],
		                      col, row_);
	}
}
