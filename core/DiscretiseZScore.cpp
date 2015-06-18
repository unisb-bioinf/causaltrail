#include "DiscretiseZScore.h"

void DiscretiseZScore::apply(unsigned int row, Data& data)
{

	unsigned int count = 0;
	float expValue = 0.0f;
	float expValue2 = 0.0f;
	for(unsigned int col = 0; col < data.input.getColCount(); col++) {
		auto value = getNumber(data.input, col, row);
		if(value) {
			expValue += value.get();
			expValue2 += (value.get() * value.get());
			++count;
		}
	}
	expValue = expValue / count;
	expValue2 = expValue2 / count;

	float standardDeviation = sqrt(expValue2 - (expValue * expValue));
	for(unsigned int col = 0; col < data.input.getColCount(); col++) {
		auto value = getNumber(data.input, col, row);

		int result = NA;
		if(value) {
			float z = std::abs((value.get() - expValue) / standardDeviation);
			result = (z > 2.0f) ? 1 : 0;
		}
		data.output.setData(result, col, row);
	}
}
