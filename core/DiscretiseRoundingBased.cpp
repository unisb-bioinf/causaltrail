#include "DiscretiseRoundingBased.h"

#include <cmath>

void DiscretiseRound::apply(unsigned int row, Data& data)
{
	DiscretiseRoundingBased::apply(row, data,
	                               static_cast<float (*)(float)>(std::round));
}

void DiscretiseCeil::apply(unsigned int row, Data& data)
{
	DiscretiseRoundingBased::apply(row, data,
	                               static_cast<float (*)(float)>(std::ceil));
}

void DiscretiseFloor::apply(unsigned int row, Data& data)
{
	DiscretiseRoundingBased::apply(row, data,
	                               static_cast<float (*)(float)>(std::floor));
}
