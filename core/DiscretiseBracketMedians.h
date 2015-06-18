#ifndef DISCRETISEBRACKETMEDIANS_H
#define DISCRETISEBRACKETMEDIANS_H

#include "Discretisations.h"

class DiscretiseBracketMedians : public Discretisations
{
	public:
	DiscretiseBracketMedians(unsigned int buckets);
	void apply(unsigned int row, Data& data) override;

	private:
	unsigned int buckets_;
};
#endif
