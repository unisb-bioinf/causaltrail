#ifndef DISCRETISETHRESHOLDBASED_H
#define DISCRETISETHRESHOLDBASED_H

#include "Discretisations.h"

/**
 * DiscretiseThresholdBased
 * Base class for Threshold Based discretisation 
 * methods
 **/
class DiscretiseThresholdBased : public Discretisations
{
	protected:
	/**apply
	 *
	 * @param row, index of the row that should be
	 * discretised
	 *
	 * @param data, struct containing data necessary
	 * for the discretisation
	 *
	 * @param threshold, the threshold to divide the
	 * data
	 *
	 * Executes the bracket medians method on the 
	 * given row in the given data
	 */
	void apply_(unsigned int row, Data& data, float threshold);
};

/**
 * DiscretiseMedian
 * Implements the Bracket Medians discretisation method
 **/
class DiscretiseMedian : public DiscretiseThresholdBased
{
	public:
	/**apply
	 *
	 * @param row, index of the row that should be
	 * discretised
	 *
	 * @param data, struct containing data necessary
	 * for the discretisation
	 *
	 * Divides the data according to its median 
	 */	
	void apply(unsigned int row, Data& data) override;
};

/**
 * DiscretiseArithmeticMean
 * Implements the Arithmetic Mean for discretisation
 **/
class DiscretiseArithmeticMean : public DiscretiseThresholdBased
{
	public:
	/**apply
	 *
	 * @param row, index of the row that should be
	 * discretised
	 *
	 * @param data, struct containing data necessary
	 * for the discretisation
	 *
	 * Divides the data according to its arithmetic
	 * mean 
	 */
	void apply(unsigned int row, Data& data) override;
};

/**
 * DiscretiseHarmonicMean
 * Implements the Harmoing Mean for discretisation
 **/
class DiscretiseHarmonicMean : public DiscretiseThresholdBased
{
	public:
	/**apply
	 *
	 * @param row, index of the row that should be
	 * discretised
	 *
	 * @param data, struct containing data necessary
	 * for the discretisation
	 *
	 * Divides the data according to its harmonic 
	 * mean 
	 */
	void apply(unsigned int row, Data& data) override;
};

/**
 * DiscretiseThreshold
 * Discretises according to a user specified
 * threshold
 **/
class DiscretiseThreshold : public DiscretiseThresholdBased
{
	public:
	DiscretiseThreshold(float threshold);
	/**apply
	 *
	 * @param row, index of the row that should be
	 * discretised
	 *
	 * @param data, struct containing data necessary
	 * for the discretisation
	 *
	 * Divides the data according to a user specified
	 * threshold
	 */
	void apply(unsigned int row, Data& data) override;

	private:
	float threshold_;
};

#endif // DISCRETISETHRESHOLDBASED_H
