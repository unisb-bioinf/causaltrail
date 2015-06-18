#ifndef DISCRETISEPT_H
#define DISCRETISEPT_H

#include "Discretisations.h"

class DiscretisePT : public Discretisations
{
	public:
	void apply(unsigned int row, Data& data) override;
};
#endif
