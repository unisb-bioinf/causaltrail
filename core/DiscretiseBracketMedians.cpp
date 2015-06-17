#include "DiscretiseBracketMedians.h"

DiscretiseBracketMedians::DiscretiseBracketMedians(unsigned int row,
                                const Matrix<std::string>& originalObservations,
                                Matrix<int>& discretisedObservations,
			        std::unordered_map<std::string,int>& observationsMap,
        			std::map<std::pair<int,int>, std::string>& observationsMapR,
				int buckets)
	:Discretisations(row,
			originalObservations,
			discretisedObservations,
			observationsMap,
			observationsMapR),
	buckets_(buckets)
{
}

void DiscretiseBracketMedians::apply(){
	const std::vector<float>& templist = createSortedVector(row_);
	std::vector<float> borderValues;
	borderValues.reserve(buckets_ + 1);
	borderValues.push_back(templist[0]);
	// Calculat Borders
	for(unsigned int i = 1; i < buckets_; i++) {
		borderValues.push_back(templist[templist.size() / buckets_ * i]);
	}
	borderValues.push_back(FLT_MAX);
	// Fill intervals
	for(unsigned int col = 0; col < templist.size(); col++) {
		float value = getNumber(col,row_);
		for(unsigned int i = 1; i <= buckets_; i++) {
			if((value >= borderValues[i - 1]) && (value <
			                                              borderValues[i])) {
				discretisedObservations_.setData(i - 1, col, row_);
				createNameEntry(i - 1, row_);
				break;
			}
		}
	}
}
