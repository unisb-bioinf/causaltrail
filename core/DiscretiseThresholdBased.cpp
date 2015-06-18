#include "DiscretiseThresholdBased.h"

#include <cmath>

void DiscretiseThresholdBased::apply_(unsigned int row,
                                      Discretisations::Data& data,
                                      float threshold)
{
	for(unsigned int col = 0; col < data.input.getColCount(); col++) {
		int result = NA;

		auto value = getNumber(data.input, col, row);
		if(value) {
			result = (value.get() > threshold) ? 1 : 0;
		}

		data.output.setData(result, col, row);
		createNameEntry(data.map, data.revMap, result, row);
	}
}

void DiscretiseMedian::apply(unsigned int row, Data& data)
{
	const std::vector<float>& templist = createSortedVector(data.input, row);
	float median;
	if(templist.size() % 2 != 0) {
		median = templist[std::ceil(templist.size() / 2)];
	} else {
		median = (templist[(templist.size() / 2) - 1] +
		          templist[templist.size() / 2]) /
		         2.0f;
	}

	apply_(row, data, median);
}

void DiscretiseArithmeticMean::apply(unsigned int row, Data& data)
{
	float mean = 0;
	unsigned int count = 0;
	for(unsigned int col = 0; col < data.input.getColCount(); col++) {
		auto value = getNumber(data.input, col, row);

		if(value) {
			mean += value.get();
			++count;
		}
	}

	apply_(row, data, mean / count);
}

void DiscretiseHarmonicMean::apply(unsigned int row, Data& data)
{
	float mean = 0;
	unsigned int count = 0;
	for(unsigned int col = 0; col < data.input.getColCount(); col++) {
		auto value = getNumber(data.input, col, row);

		if(value) {
			mean += 1.0f / value.get();
			++count;
		}
	}

	apply_(row, data, count / mean);
}

DiscretiseThreshold::DiscretiseThreshold(float threshold)
    : threshold_(threshold)
{
}

void DiscretiseThreshold::apply(unsigned int row, Data& data)
{
	apply_(row, data, threshold_);
}
