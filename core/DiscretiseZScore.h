#ifndef DISCRETISEZSCORE_H
#define DISCRETISEZSCORE_H

#include "Discretisations.h"

class DiscretiseZScore : public Discretisations
{
	public:
	void apply(unsigned int row, Data& data) override;
};
#endif
