#ifndef DISCRETISEROUNDINGBASED_H
#define DISCRETISEROUNDINGBASED_H

#include "Discretisations.h"

/**
 * Base class of rounding-based discretisation methods.
 */
class DiscretiseRoundingBased : public Discretisations
{
	protected:
	/**apply
	 *
	 * @param row index of the row that should be
	 * discretised
	 *
	 * @param data struct containing data necessary
	 * for the discretisation
	 *
	 * Executes the rounding function func on the data
	 */
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

/**
 * Implements the standard round function
 */
class DiscretiseRound : public DiscretiseRoundingBased
{
	public:
	/**apply
	 *
	 * @param row index of the row that should be
	 * discretised
	 *
	 * @param data struct containing data necessary
	 * for the discretisation
	 *
	 * Rounds the original data to the nearest integer
	 * value
	 */
	void apply(unsigned int row, Data& data) override;
};

/**
 * Implements the Ceil function for discretisation
 */
class DiscretiseCeil : public DiscretiseRoundingBased
{
	public:
	/**apply
	 *
	 * @param row index of the row that should be
	 * discretised
	 *
	 * @param data struct containing data necessary
	 * for the discretisation
	 *
	 * Executes ceil on the original data 
	 */
	void apply(unsigned int row, Data& data) override;
};

/**
 * Implements the Floor function for discretisation
 */
class DiscretiseFloor : public DiscretiseRoundingBased
{
	public:
	/**apply
	 *
	 * @param row index of the row that should be
	 * discretised
	 *
	 * @param data struct containing data necessary
	 * for the discretisation
	 *
	 * Executes Floor on the original data 
	 */
	void apply(unsigned int row, Data& data) override;
};

#endif // DISCRETISEROUNDINGBASED_H
