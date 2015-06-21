#ifndef DISCRETISEBRACKETMEDIANS_H
#define DISCRETISEBRACKETMEDIANS_H

#include "Discretisations.h"

/**
 * DiscretiseBracketMedians
 * Implements the Bracket Medians discretisation method
 **/
class DiscretiseBracketMedians : public Discretisations
{
	public:
	/**
	 *
	 * @param unsigned int buckets number of buckets
	 * in which the original values should be divided
	 *
	 * @return A DiscretiseBracketMediansObject
	 */
	DiscretiseBracketMedians(unsigned int buckets);
	
	/**apply
	 *
	 * @param row, index of the row that should be
	 * discretised
	 *
	 * @param data, struct containing data necessary
	 * for the discretisation
	 *
	 * Executes the bracket medians method on the 
	 * given row in the given data
	 */
	void apply(unsigned int row, Data& data) override;

	private:
	unsigned int buckets_;
};
#endif
