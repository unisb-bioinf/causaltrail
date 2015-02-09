#ifndef DISCRETISER_H
#define DISCRETISER_H

#include "Network.h"
#include "float.h"
#include <map>

class Discretiser{
	public: 
	/**Discretiser
 	 *
	 * @param originalObservations, a const reference to the matrix containing the raw sample data
	 * @param obsMatrix, a reference to the new observation matrix that shall contain the discretised data
	 * @param network, a reference to the network
	 *
 	 * @return Discretiser Object
	 *
 	 *
 	 */
	Discretiser(const Matrix<std::string>& originalObservations, Matrix<int>& obsMatrix, Network& network);

	/**Discretiser
 	 *
	 * @param originalObservations, a const reference to the matrix containing the raw sample data
	 * @param filename, name of a "controlFile" that regulates the discretisation for each node 
	 * @param obsMatrix, a reference to the new observation matrix that shall contain the discretised data
	 * @param network, a reference to the network
	 *
 	 * @return Discretiser Object
 	 *
	 * In contrast to the before declared constructor, this constructor uses the controlFile and automatically discretises
	 * all observations that are listed in this file
 	 */
	Discretiser(const Matrix<std::string>& originalObservations,const std::string& filename, Matrix<int>& obsMatrix, Network& network);
	
	/**discretiseRow
 	 *
	 * @param row, row to discretise
	 * @param method, ID of the method to use for discretisatoin
	 * @param threshold, obtional argument needed for some discretisation methods
	 *
 	 */
	void discretiseRow(unsigned int row, unsigned int method, float threshold);

	/**getEntry
 	 *
	 * @param col, colum of interest
	 * @param row, row of interest
	 *
 	 * @return Returns an integer representation for the discretised datapoint at the given position
	 *
 	 */
	int getEntry(unsigned int col, unsigned int row);

	private:

	/**discretise
 	 *
	 * @param filename, name of the controlFile
	 *
 	 * Discretises each row as specified in the controlFile. The structure of the file
	 * is as follows:
	 * Row	MethodCode
	 *
	 * Row numbers start at 0.
 	 *
 	 * The method integer codes are:
	 *	0: discretiseCeil
	 *	1: discretiseFloor
	 *	2: discretiseRound
	 *	3: discretiseByAMean
	 *	4: discretiseByHMean
	 *	5: discretiseByMedian
	 *	6: discretiseManually
	 *	7: discretiseBracketMedians
	 *	8: discretisePearsonTukey
	 *	9: mapNamesToInt
 	 */
	void discretise(const std::string& filename);

	/**discretiseFloor
 	 *
	 * @param row, row to discretise
	 *
 	 * Discretises the given row by rounding each value to the next smaller integer value
 	 */
	void discretiseFloor(unsigned int row);

	/**discretiseZ
 	 *
	 * @param row, row to discretise
	 *
 	 * Discretises the given row by calculating the z-score for each feature and discretising accordingly.
 	 */
	void discretiseZ(unsigned int row);

	/**discretiseCeil
 	 *
	 * @param row, row to discretise
	 *
 	 * Discretises the given row by rounding each value to the next larger integer value
 	 */
	void discretiseCeil(unsigned int row);

	/**discretiseRound
 	 *
	 * @param row, row to discretise
	 *
 	 * Discretises the given row by rounding each value according to the first digit after the comma (<5, or >=5)
 	 */
	void discretiseRound(unsigned int row);

	/**discretiseByAMean
 	 *
	 * @param row, row to discretise
	 *
 	 * Discretises the given row by computing the arithmetic mean of it and comparing each value against it
 	 */
	void discretiseByAMean(unsigned int row);

	/**discretiseByHMean
 	 *
	 * @param row, row to discretise
	 *
 	 * Discretises the given row by computing the harmonic mean of it and comparing each value against it
 	 */
	void discretiseByHMean(unsigned int row);

	/**discretiseByMedian
 	 *
	 * @param row, row to discretise
	 *
 	 * Discretises the given row by computing the median of it and comparing each value against it
 	 */
	void discretiseByMedian(unsigned int row);

	/**discretiseManually
 	 *
	 * @param row, row to discretise
	 * @param threshold, a manually chossen threshold that divids the data into two groups
	 *
 	 * Discretises the given row by comparing each value against the given threshold
 	 */
	void discretiseManually(unsigned int row, float threshold);

	/**discretiseBracketMedians
 	 *
	 * @param row, row to discretise
	 * @param numberOfBrackets, number of brackets to divide the data into
	 *
 	 * Performs the BracketMedians method to discretise the data
 	 */
	void discretiseBracketMedians(unsigned int row, unsigned int numberOfBrackets);

	/**discretisePearsonTukey
 	 *
	 * @param row, row to discretise
	 *
 	 * Performs the PearsonTukey method to discretise the given row
 	 */
	void discretisePearsonTukey(unsigned int row);

	/**mapNamesToInt
 	 *
	 * @param row, row to discretise
	 *
 	 * Maps discrete values in the given row from a string to a integer representation
 	 */
	void mapNamesToInt(unsigned int row);

	/**getNumber
 	 *
	 * @param col, column of intererst
	 * @param row, row of interest
 	 *
	 * Converts the string at the given position into a float value
 	 */
	float getNumber(unsigned int col, unsigned int row);

	/**convertToDenseNumbers,
 	 *
	 * @param row, row to convert
	 *
 	 * Converts the entries in a given row to dense numbers
 	 */
	void convertToDenseNumbers(unsigned int row);

	/**createNameEntry
 	 *
	 * @param value, value to convert
	 * @param row, row in focus
	 *
 	 * Creates an entry in the ObservationsMap_ and the ObservationsMapR_ for the given value and the given row
 	 */
	void createNameEntry(int value, unsigned int row);

	/**adaptFormat
 	 *
 	 * Replaces various kinds of NA representations by a unqiue version
 	 */
	void adaptFormat();

	/**createSortedVector
 	 *
	 * @param row, row to sort
	 *
 	 * @return a sorted vector (small -> large)  containing the entries of the given row
 	 */
	std::vector<float> createSortedVector(unsigned int row);
	//Matrix containing the original raw sample data
	Matrix<std::string> originalObservations_;
	//Matrix containing the discretised data
	Matrix<int>& observations_;
	//Map from original value to integer representation
	std::unordered_map<std::string,int>& observationsMap_;
	//Map from integer representation (and observationRow from class Node) to the original representation
    std::map<std::pair<int,int>, std::string>& observationsMapR_;
};
#endif
