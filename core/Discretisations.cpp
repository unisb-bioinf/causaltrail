#include "Discretisations.h"

const int Discretisations::NA = -1;

boost::optional<float> Discretisations::getNumber(const Observations& obs,
                                                  unsigned int col,
                                                  unsigned int row)
{
	const auto& temp = obs(col, row);

	if(temp == "NA") {
		return boost::none;
	}

	return std::stof(temp);
}

void Discretisations::createNameEntry(ObservationMap& obs,
                                      RevObservationMap& obsR, int value,
                                      unsigned int row)
{
	std::string svalue = std::to_string(value);
	obs[svalue] = value;
	obsR[std::make_pair(value, row)] = svalue;
}

std::vector<float> Discretisations::createSortedVector(const Observations& obs,
                                                       unsigned int row)
{
	std::vector<float> templist;
	templist.reserve(obs.getColCount());
	for(unsigned int col = 0; col < obs.getColCount(); col++) {
		auto value = getNumber(obs, col, row);

		if(value) {
			templist.push_back(value.get());
		}
	}
	std::sort(templist.begin(), templist.end());
	return templist;
}

void Discretisations::convertToDenseNumbers(
    const std::vector<boost::optional<int>>& discretized, Data& data,
    unsigned int row)
{
	std::vector<boost::optional<int>> tmp(discretized);
	std::sort(tmp.begin(), tmp.end(), [](auto a, auto b) {
		if(a && b) {
			return a.get() < b.get();
		}

		return bool(a);
	});

	tmp.erase(std::unique(tmp.begin(), tmp.end()), tmp.end());
	if(tmp.back() == boost::none) {
		tmp.pop_back();
	}

	for(unsigned int col = 0; col < data.input.getColCount(); ++col) {
		int result = NA;
		if(discretized[col]) {
			for(size_t i = 0; i < tmp.size(); ++i) {
				if(discretized[col] == tmp[i]) {
					result = i;
					break;
				}
			}
		}

		data.output.setData(result, col, row);
		createNameEntry(data.map, data.revMap, result, row);
	}
}
