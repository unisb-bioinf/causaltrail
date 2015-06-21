#ifndef DISCRETISEZSCORE_H
#define DISCRETISEZSCORE_H

#include "Discretisations.h"

/**
 * DiscretiseZSCORE
 * Uses the Z-Score for discretisation
 **/
class DiscretiseZScore : public Discretisations
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
	 * Divides the data according to its Z-Score
	 */
	void apply(unsigned int row, Data& data) override;
};
#endif
