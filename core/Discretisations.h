#ifndef DISCRETISATIONS_H
#define DISCRETISATIONS_H

include "Matrix.h" 

class Discretisations {
	public:
	Discretisations(unsigned int row, const Matrix<std::string>& originalObservations, Matrix<int>& discretisedObservations);
	virtual void apply();	

	private:
	unsigned int row_;
	const Matrix<std::string>& originalObservations_;
	Matrix<int>& discretisedObservations_;
};
#endif
