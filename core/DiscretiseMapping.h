#ifndef DISCRETISEMAPPING_H
#define DISCRETISEMAPPING_H

#include "Discretisations.h"

/**
 * DiscretiseMapping
 * Mapps already discrete values to a dense integer representation
 **/
class DiscretiseMapping : public Discretisations
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
	 * Executes a mapping from discrete values to a
	 * dense integer representation 
	 */
	void apply(unsigned int row, Data& data) override;
};
#endif
