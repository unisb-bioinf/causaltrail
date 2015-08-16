#include "DiscretiseBracketMedians.h"

DiscretiseBracketMedians::DiscretiseBracketMedians(unsigned int buckets)
    : buckets_(buckets)
{
}

void DiscretiseBracketMedians::apply(unsigned int row, Data& data)
{
	const auto& templist = createSortedVector(data.input, row);
	std::vector<float> borderValues;
	borderValues.reserve(buckets_ + 1);
	// Calculate borders
	borderValues.push_back(templist[0]);
	for(unsigned int i = 1; i < buckets_; i++) {
		float borderValue=templist[templist.size() / buckets_ * i];
		borderValues.push_back(borderValue);
	}
	borderValues.push_back(std::numeric_limits<float>::max());

	// Fill intervals
	for(unsigned int col = 0; col < data.input.getColCount(); col++) {
		auto value = getNumber(data.input, col, row);

		int result = NA;

		if(value) {
			for(unsigned int i = 1; i <= buckets_; i++) {
				if(value.get() >= borderValues[i - 1] &&
				   value.get() < borderValues[i]) {
					result = i - 1;
					break;
				}
			}
		}

		data.output.setData(result, col, row);
		createNameEntry(data.map, data.revMap, result, row);
	}
}
