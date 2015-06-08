#ifndef DISCRETISATIONS_H
#define DISCRETISATIONS_H

#include "Matrix.h" 
#include "map"
class Discretisations {
	public:
	Discretisations(unsigned int row, 
			const Matrix<std::string>& originalObservations, 
			Matrix<int>& discretisedObservations,
			std::unordered_map<std::string,int>& observationsMap,
		        std::map<std::pair<int,int>, std::string>& observationsMapR);

	virtual void apply();	

	protected:
	float getNumber(unsigned int col, unsigned int row);

	void createNameEntry(int value, unsigned int row);

	std::vector<float> createSortedVector(unsigned int row);

	void convertToDenseNumbers(unsigned int row);

	unsigned int row_;

	const Matrix<std::string>& originalObservations_;

	Matrix<int>& discretisedObservations_;

        std::unordered_map<std::string,int>& observationsMap_;

        std::map<std::pair<int,int>, std::string>& observationsMapR_;

};
#endif
