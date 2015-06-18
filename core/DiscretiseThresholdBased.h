#ifndef DISCRETISETHRESHOLDBASED_H
#define DISCRETISETHRESHOLDBASED_H

#include "Discretisations.h"

class DiscretiseThresholdBased : public Discretisations
{
	protected:
	void apply_(unsigned int row, Data& data, float threshold);
};

class DiscretiseMedian : public DiscretiseThresholdBased
{
	public:
	void apply(unsigned int row, Data& data) override;
};

class DiscretiseArithmeticMean : public DiscretiseThresholdBased
{
	public:
	void apply(unsigned int row, Data& data) override;
};

class DiscretiseHarmonicMean : public DiscretiseThresholdBased
{
	public:
	void apply(unsigned int row, Data& data) override;
};

class DiscretiseThreshold : public DiscretiseThresholdBased
{
	public:
	DiscretiseThreshold(float threshold);
	void apply(unsigned int row, Data& data) override;

	private:
	float threshold_;
};

#endif // DISCRETISETHRESHOLDBASED_H