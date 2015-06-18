#ifndef DISCRETISEMAPPING_H
#define DISCRETISEMAPPING_H

#include "Discretisations.h"

class DiscretiseMapping : public Discretisations
{
	public:
	void apply(unsigned int row, Data& data) override;
};
#endif
