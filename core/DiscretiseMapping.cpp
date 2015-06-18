#include "DiscretiseMapping.h"

void DiscretiseMapping::apply(unsigned int row, Data& data)
{
	const auto& uniqueValues = data.input.getUniqueRowValues(row);

	size_t index = 0;
	for(const auto& value : uniqueValues) {
		int result = NA;
		if(value != "NA") {
			result = index++;
		}

		data.map[value] = result;
		data.revMap[std::make_pair(result, row)] = value;
	}

	for(unsigned int col = 0; col < data.input.getColCount(); col++) {
		data.output.setData(data.map[data.input(col, row)], col, row);
	}
}
