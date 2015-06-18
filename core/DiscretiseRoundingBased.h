#ifndef DISCRETISEROUNDINGBASED_H
#define DISCRETISEROUNDINGBASED_H

#include "Discretisations.h"

class DiscretiseRoundingBased : public Discretisations
{
	protected:
	template <typename T>
	void apply(unsigned int row, Data& data, const T& func)
	{

		std::vector<boost::optional<int>> discretised(data.input.getColCount(),
		                                              boost::none);

		for(unsigned int col = 0; col < data.input.getColCount(); col++) {
			auto value = getNumber(data.input, col, row);

			if(value) {
				discretised[col] = func(value.get());
			}
		}

		convertToDenseNumbers(discretised, data, row);
	}
};

class DiscretiseRound : public DiscretiseRoundingBased
{
	public:
	void apply(unsigned int row, Data& data) override;
};

class DiscretiseCeil : public DiscretiseRoundingBased
{
	public:
	void apply(unsigned int row, Data& data) override;
};

class DiscretiseFloor : public DiscretiseRoundingBased
{
	public:
	void apply(unsigned int row, Data& data) override;
};

#endif // DISCRETISEROUNDINGBASED_H
