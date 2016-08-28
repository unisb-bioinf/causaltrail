#ifndef DISCRETISEMAPPING_H
#define DISCRETISEMAPPING_H

#include "Discretisations.h"

/**
 * Maps already discrete values to a dense integer representation
 */
class DiscretiseMapping : public Discretisations
{
	public:
	/**
	 * Executes a mapping from discrete values to a
	 * dense integer representation
	 *
	 * @param row index of the row that should be
	 * discretised
	 *
	 * @param data struct containing data necessary
	 * for the discretisation
	 *
	 */
	void apply(unsigned int row, Data& data) override;
};

#endif
