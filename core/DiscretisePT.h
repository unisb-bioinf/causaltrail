#ifndef DISCRETISEPT_H
#define DISCRETISEPT_H

#include "Discretisations.h"

/**
 * DiscretisePT
 * Implements the Pearson Tuckey Method
 **/
class DiscretisePT : public Discretisations
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
	 * Executes the Pearson Tuckey Method on the 
	 * given row in the given data
	 */
	void apply(unsigned int row, Data& data) override;
};
#endif
