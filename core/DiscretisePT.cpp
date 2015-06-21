#include "DiscretisePT.h"

void DiscretisePT::apply(unsigned int row, Data& data)
{
	const std::vector<float>& templist = createSortedVector(data.input, row);
	const std::vector<float> borderValues = {
	    // Calculate borders: Constants are defined by the method
	    std::numeric_limits<float>::min(),
	    templist[ceil(0.185 * templist.size())-1],
	    templist[ceil(0.815 * templist.size())-1],
	    std::numeric_limits<float>::max()};
	// Fill intervals
	for(unsigned int col = 0; col < data.input.getColCount(); col++) {
		auto value = getNumber(data.input, col, row);

		if(!value) {
			data.output.setData(NA, col, row);
			continue;
		}

		for(int i = 1; i < 4; i++) {
			if(value.get() >= borderValues[i - 1] &&
			   value.get() < borderValues[i]) {
				data.output.setData(i - 1, col, row);
				createNameEntry(data.map, data.revMap, i - 1, row);
				break;
			}
		}
	}
}
