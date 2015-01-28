#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <unordered_map>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

template <typename T> class Matrix;
template <typename T> std::ostream& operator<<(std::ostream&, const Matrix<T>&);

template <typename T> class Matrix
{
	public:
	/**Detailed Constructor
	 *
	 * @param filename The file containing the data to be stored in the matrix
	 * @param colNames Flag to indicate the existence of column names
	 * @param rowNames Flag to indicate the existence of row names
	 * @param initialValue Initial value for all points in the matrix
	 *
	 * @return A Matrix Object
	 *	
	 * Creates a Matrix containing the data from the specified file
	 */
	Matrix(std::string filename, bool colNames, bool rowNames, T initialValue);

	/**Detailed Constructor
	 *
	 * @param colNames A vector containing the column names
	 * @param rowNames A vector containing the row names
	 * @param initialValue Initial value for all points in the matrix
	 *
	 * @return A Matrix Object
	 *
	 * Creates a Matrix with the specified name vectors. The size of the matrix is calculated 
	 * using the number of entries in the name vectors.
	 */
	Matrix(std::vector<std::string> colNames = {"NA"},
	       std::vector<std::string> rowNames = {"NA"}, T initialValue = NULL);

	/**Detailed Constructor
	 *
	 * @param colCount Number of columns
	 * @param rowCount Number of rows
	 * @param initialValue Initial value for all points in the matrix
	 * @param colNames (obtional)
	 * @param rowNames (obtional)
	 * @return A Matrix Object
	 *
	 * Creates a Matrix with the specified number of columns and rows
	 */
	Matrix(int colCount = 0, int rowCount = 0, T initialValue = NULL,
	       std::vector<std::string> colNames = {"NA"},
	       std::vector<std::string> rowNames = {"NA"});

	/**Operator()
	 *
	 * @param col Column number
	 * @param row Row number
	 *
	 * @return Value at position Matrix[col,row]
	 *
	 * This operator returns the matrix value at the specified position
	 */
	T& operator()(unsigned int col, unsigned int row);

	/**Operator() const
	*
	* @param col Column number
	* @param row Row number
	*
	* @return Value at position Matrix[col,row]
	*
	* This operator returns the matrix value at the specified position
	*/
	const T& operator()(unsigned int col, unsigned int row) const;

	/**Operator <<
	 *
	 * @param os Reference to an ostream
	 * @param m Reference to a matrix object
	 *
	 * @return ostream Containing the content of the matrix
	 *
	 * This operator adds the content of the matrix to an ostream.
	 * Column and Rownames are included if they are  available.
	 *
	 */
	friend std::ostream& operator<<<>(std::ostream& os, const Matrix<T>& m);

	/**setData
	 *
	 * @param value Value to be added into the matrix
	 * @param col Destination column
	 * @param row Destination row
	 *
	 * This method stores a value in the matrix at the specified position
	 *
	 * @throw Exception
	 */
	void setData(T value, unsigned int col, unsigned int row);

	/**setRowNames
	 *
	 * @param names Vector containing row names
	 *
	 * This method stors the rownames of the matrix. It also generates a mapping
	 * between rownames and index
	 *
	 */
	void setRowNames(const std::vector<std::string>& names);

	/**setColNames
	 *
	 * @param names vector containing column names
	 *
	 * This method stors the rownames of the matrix. It also generates a mapping
	 *between
	 * columnames and index
	 *
	 */
	void setColNames(const std::vector<std::string>& names);

	/**getRowCount
	 *
	 * @return Number of rows
	 *
	 * This function returns the number of rows of the matrix
	 */
	unsigned int getRowCount();

	/**getColCount
	 *
	 * @return Number of columns
	 *
	 * This function returns the number of columns
	 */
	unsigned int getColCount();

	/**getRowCount const
	 *
	 * @return Number of rows
	 *
	 * This function returns the number of rows of the matrix
	 */
	unsigned int getRowCount() const;

	/**getColCount const
	 *
	 * @return Number of columns
	 *
	 * This function returns the number of columns
	 */
	unsigned int getColCount() const;

	/**getValueByNames
  	 *
     * @param colName Name of the query column
	 * @param rowName Name of the query row
	 *
	 * @return Value at position Matrix[colName, rowName]
	 *
	 * This function allows access to the matrix via col- and rownames
	 *	
	 * @throw Exception Invalid_argument(Either column or row name are invalid)
	 */
	T& getValueByNames(const std::string& colName, const std::string& rowName);
	
	/**getValueByNames const
	 *
	 * @param colName Name of the query column
	 * @param rowName Name of the query row
	 *
	 * @return Value at position Matrix[colName, rowName]
	 *
	 * This function allows access to the matrix via col- and rownames
	 *	
	 * @throw Exception Invalid_argument(Either column or row name are invalid)
	 */
	const T& getValueByNames(const std::string& colName, const std::string& rowName) const;

	/**getRowNames
	 *
	 * @return rowNames
	 *
	 * Returns a reference to the row names of the matrix
	 */
	std::vector<std::string>& getRowNames();

	/**getRowNames const
	 *
	 * @return rowNames
	 *
	 * Returns a reference to the row names of the matrix
	 */
	const std::vector<std::string>& getRowNames() const;

	/**getColNames
	 *
	 * @return column names
	 *
	 * Returns a reference to the column names of the matrix
	 */
	std::vector<std::string>& getColNames();
	
	/**getColNames const
	 *
	 * @return column names
	 *
	 * Returns a reference to the column names of the matrix
	 */
	const std::vector<std::string>& getColNames() const;

	/**findRow
	 *
	 * @param element Query name of the row
	 *
	 * @return row index containing the query value, -1 if the matrix does not
	 * contain such a row
	 *
	 * This function returns the index of the row, which matches the given row name
	 */
	int findRow(const std::string& element);

	/**findCol
	 *
	 * @param element Query name of the col
	 *
	 * @return col index containing the query value, -1 if the matrix does not
	 * contain such a col
	 *
	 * This function returns the index of the col, which matches the given col name
	 */
	int findCol(const std::string& element);

	/**findRow const
	 *
	 * @param element Query name of the row
	 *
	 * @return row index containing the query value, -1 if the matrix does not
	 * contain such a row
	 *
	 * This function returns the index of the row, which matches the given row name
	 */
	int findRow(const std::string& element) const;

	/**findCol const
	 *
	 * @param element Query name of the col
	 *
	 * @return col index containing the query value, -1 if the matrix does not
	 * contain such a col
	 *
	 * This function returns the index of the col, which matches the given col name
	 */
	int findCol(const std::string& element) const;

	/**calculateRowSum
	 *
	 * @param row Index of the row of interest
	 *
	 * @return col sum of the values
	 *
	 * If a matrix contains numerical values, this method computes the sum of
	 * those values that are assigned to the specified row
	 */
	float calculateRowSum(unsigned int row);

	/**calculateRowSum const
	 *
	 * @param row Index of the row of interest
	 *
	 * @return col sum of the values
	 *
	 * If a matrix contains numerical values, this method computes the sum of
	 * those values that are assigned to the specified row
	 */
	float calculateRowSum(unsigned int row) const;

	/**calculateColSum
	 *
	 * @param col Index of the row of interest
	 *
	 * @return col sum of the values
	 *
	 * If a matrix contains numerical values, this method computes the sum of
	 * those values that are assigned to the specified column
	 */
	float calculateColSum(unsigned int col);
	/**calculateColSum const 
	 *
	 * @param col Index of the col of interest
	 *
	 * @return col sum of the values
	 *
	 * If a matrix contains numerical values, this method computes the sum of
	 * those values that are assigned to the specified column
	 */
	float calculateColSum(unsigned int col) const;

	/**getUniqueRowValues
	 *
	 * @param row index
	 *
	 * @return vector containing the unique elements stored in the query row of the
	 * matrix
	 *
	 * This function returns the unique elements which are stored in the specified
	 * row of the matrix
	 */
	std::vector<T> getUniqueRowValues(unsigned int row);

	/**getUniqueRowValues
	 *
	 * @param row index
	 * @param exclud A value to exclud
	 *
	 * @return vector containing the unique elements stored in the query row of the
	 * matrix
	 *
	 * This function returns the unique elements which are stored in the specified
	 * row of the matrix
	 */
	std::vector<T> getUniqueRowValues(unsigned int row, const T& exclud);

	/**getUniqueColValues
	 *
	 * @param row index
	 *
	 * @return vector containing the unique elements stored in the query row of the
	 * matrix
	 *
	 * This function returns the unique elements which are stored in the specified
	 * row of the matrix
	 */
	std::vector<T> getUniqueColValues(unsigned int col);

	/**getUniqueColValues
	 *
	 * @param row index
	 * @param exclud A value to exclud
	 *
	 * @return vector containing the unique elements stored in the query row of the
	 * matrix
	 *
	 * This function returns the unique elements which are stored in the specified
	 * row of the matrix
	 */
	std::vector<T> getUniqueColValues(unsigned int col, const T& exclud);

	/**getUniqueRowValues const
	 *
	 * @param row index
	 *
	 * @return vector containing the unique elements stored in the query row of the
	 * matrix
	 *
	 * This function returns the unique elements which are stored in the specified
	 * row of the matrix
	 */
	std::vector<T> getUniqueRowValues(unsigned int row) const;

	/**getUniqueRowValues const
	 *
	 * @param row index
	 * @param exclud A value to exclud
	 *
	 * @return std::vector<T> A vector containing the unique elements stored in the query row of the
	 * matrix
	 *
	 * This function returns the unique elements which are stored in the specified
	 * row of the matrix
	 */
	std::vector<T> getUniqueRowValues(unsigned int row, const T& exclud) const;

	/**getUniqueColValues const
	 *
	 * @param row index
	 *
	 * @return vector containing the unique elements stored in the query row of the
	 * matrix
	 *
	 * This function returns the unique elements which are stored in the specified
	 * row of the matrix
	 */
	std::vector<T> getUniqueColValues(unsigned int col) const;

	/**getUniqueColValues const
	 *
	 * @param col index
	 * @param exclud A value to exclud 
	 *
	 * @return vector containing the unique elements stored in the query row of the
	 * matrix
	 *
	 * This function returns the unique elements which are stored in the specified
	 * row of the matrix
	 */
	std::vector<T> getUniqueColValues(unsigned int col, const T& exclud) const;

	/**contains const
	 *
	 * @param query Query value
	 *
	 * @return true, if the matrix contains the query element, false otherwise
	 *
	 * This function checks, whether the query element is stored in the matrix or
	 * not
	 */	
	bool contains(const T& query) const;

	/**readMatrix
	 *
	 * @param filename File that should be read
	 * @param rowNames Flag indicating whether the matrix contains rowNames
	 * @param colNames Flag indicating whether the matrix contains colNames
	 * @param initialValue value that should be used for initilization
	 *
	 * This methods reads a tab or space delimited file containing a matrix.
	 */
	void readMatrix(const std::string& filename, bool colNames, bool rowNames,
	                const T& initialValue);

	/**countElement
	 *
	 * @param colrow Control variable defining whether rows or columns should be
	 * analysed: 1=row, 0=col
	 * @param number index of the row or column which should be analysed
	 * @param t query
	 *
	 * @return the number of occurence of element t in the specified column or row
	 * of the matrix
	 *
	 * This function counts how often a certain element is stored in a specified
	 * column or row of the matrix
	 */
	unsigned int countElement(unsigned int colrow, unsigned int number,const T& t);
 
	/**countElement const
	 *
	 * @param colrow control variable defining whether rows or columns should be
	 * analysed 1=row, 0=col
	 * @param number Index of the row or column which should be analysed
	 * @param t query
	 *
	 * @return the number of occurence of element t in the specified column or row
	 * of the matrix
	 *
	 * This function counts how often a certain element is stored in a specified
	 * column or row of the matrix
	 */

	unsigned int countElement(unsigned int colrow, unsigned int number,const T& t) const;	

	/**containsElement
	 *
	 * @param colrow control variable defining whether rows or columns should be
	 * analysed 1=row, 0=col
	 * @param number index of the row or column which should be analysed
	 * @param t query
	 *
	 * @return 0 if Element is not contained in the specified row or colum, 1
	 * otherwise
	 *
	 * The method checks wheter a certain element is contained in a row or a column
	 */
	bool containsElement(unsigned int colrow, unsigned int number,const T& t);

	/**containsElement
	 *
	 * @param colrow control variable defining whether rows or columns should be
	 * analysed 1=row, 0=col
	 * @param number index of the row or column which should be analysed
	 * @param t query
	 *
	 * @return 0 if Element is not contained in the specified row or colum, 1
	 *otherwise
	 *
	 * The method checks wheter a certain element is contained in a row or a column
	 */
	bool containsElement(unsigned int colrow, unsigned int number, const T& t) const;	

	/**resize
	 *
	 * @param colCount New number of columns
	 * @param rowCount New number of rows
	 * @param initialValue Value that should be used for initialisation
	 *
	 * This function adapts the size of the matrix. A matrix can not be shrinked,
	 *only enlarged.
	 * Existing elements will stay at their original matrix position.
	 *
	 * @throw Exception invalid_argument(If the original number of colums or rows is
	 * smaller than the new one)
	 */
	void resize(unsigned int colCount, unsigned int rowCount, T initalValue);

	/**hasNACol
	 * 
	 * @return bool
	 * 
	 * Returns true if NA is contained in a column of the matrix
     */
	bool hasNACol();

	/**hasNARow
	 *
	 * @return bool
	 *
	 * Returns true if NA is contained in a column of the row
	 */
	bool hasNARow();

	/**hasNACol
	 * 
	 * @return bool
	 * 
	 * Returns true if NA is contained in a column of the matrix
     */
	bool hasNACol() const;

	/**hasNARow
	 *
	 * @return bool
	 *
	 * Returns true if NA is contained in a column of the row
	 */
	bool hasNARow() const;

	/**clear
	 *
	 * Reset operation for the matrix
	 */
	void clear();

	private:
	//Unsigned ints to store the size of the matrix
	unsigned int rowCount_;
	unsigned int colCount_;
	//Vectors to store the rowNames
	std::vector<std::string> rowNames_;
	std::vector<std::string> colNames_;
	//Unordered maps to hash from Names to their corresponding index
	std::unordered_map<std::string, unsigned int> rowNamesToIndex_;
	std::unordered_map<std::string, unsigned int> colNamesToIndex_;
	//Vector to store the actual data
	std::vector<T> data_;
};

template <typename T>
Matrix<T>::Matrix(std::string filename, bool colNames, bool rowNames,
                  T initialValue)
{
	colCount_ = 0;
	rowCount_ = 0;
	readMatrix(filename, colNames, rowNames, initialValue);
}

template <typename T>
Matrix<T>::Matrix(std::vector<std::string> colNames,
                  std::vector<std::string> rowNames, T initialValue)
    : rowCount_(rowNames.size()),
      colCount_(colNames.size()),
      data_(rowNames.size() * colNames.size(), initialValue)
{
	setColNames(colNames);
	setRowNames(rowNames);
}

template <typename T>
Matrix<T>::Matrix(int colCount, int rowCount, T initialValue,
                  std::vector<std::string> colNames,
                  std::vector<std::string> rowNames)
    : rowCount_(rowCount),
      colCount_(colCount),
      data_(rowCount * colCount, initialValue)
{
	setColNames(colNames);
	setRowNames(rowNames);
}

template <typename T>
T& Matrix<T>::operator()(unsigned int col, unsigned int row)
{
	if((col > colCount_)or(row > rowCount_)) {
		throw std::invalid_argument("In Matrix(), Invalid matrix position");
	}
	return data_[col + row * colCount_];
}

template <typename T>
const T& Matrix<T>::operator()(unsigned int col, unsigned int row) const
{
	if((col > colCount_)or(row > rowCount_)) {
		throw std::invalid_argument(
		    "In Matrix() const, Invalid matrix position" +
		    boost::lexical_cast<std::string>(col) + " " +
		    boost::lexical_cast<std::string>(row));
	}
	return data_[col + row * colCount_];
}

template <typename T>
T& Matrix<T>::getValueByNames(const std::string& colName,
                              const std::string& rowName)
{
	int col = findCol(colName);
	int row = findRow(rowName);
	if((row == -1)or(col == -1)) {
		throw std::invalid_argument("Specified elements not found");
	} else
		return data_[col + row * colCount_];
}

template <typename T>
const T& Matrix<T>::getValueByNames(const std::string& colName,
                              const std::string& rowName) const
{
	int col = findCol(colName);
	int row = findRow(rowName);
	if((row == -1)or(col == -1)) {
		throw std::invalid_argument("Specified elements not found");
	} else
		return data_[col + row * colCount_];
}
template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m)
{ // Write Column Names
	os << "\t";
	for(std::vector<std::string>::const_iterator it = m.colNames_.begin();
	    it != m.colNames_.end(); ++it)
		os << *it << "\t";
	os << "\n";
	// Write Content & Row Names
	for(int row = 0; row < m.rowCount_; row++) {
		if(row < m.rowNames_.size())
			os << m.rowNames_[row] << "\t";
		else
			os << "\t";
		for(int col = 0; col < m.colCount_; col++)
			os << m.data_[col + row * m.colCount_] << "\t";
		if(row < m.rowCount_ - 1)
			os << "\n";
	}
	return os;
}

template <typename T>
void Matrix<T>::setData(T value, unsigned int col, unsigned int row)
{
	if((col > colCount_)or(row > rowCount_)) {
		throw std::invalid_argument("In setData, Invalid matrix position");
	}
	data_[col + row * colCount_] = value;
}

template <typename T>
void Matrix<T>::setRowNames(const std::vector<std::string>& names)
{
	rowNames_ = names;
	unsigned int counter = 0;
	for(const auto& n : names) {
		rowNamesToIndex_[n] = counter;
		counter++;
	}
}

template <typename T>
void Matrix<T>::setColNames(const std::vector<std::string>& names)
{
	colNames_ = names;
	unsigned int counter = 0;
	for(const auto& n : names) {
		colNamesToIndex_[n] = counter;
		counter++;
	}
}

template <typename T> unsigned int Matrix<T>::getRowCount()
{
	return rowCount_;
}

template <typename T> unsigned int Matrix<T>::getColCount()
{
	return colCount_;
}

template <typename T> unsigned int Matrix<T>::getRowCount() const
{
	return rowCount_;
}

template <typename T> unsigned int Matrix<T>::getColCount() const
{
	return colCount_;
}

template <typename T> std::vector<std::string>& Matrix<T>::getRowNames()
{
	return rowNames_;
}

template <typename T> std::vector<std::string>& Matrix<T>::getColNames()
{
	return colNames_;
}

template <typename T>
const std::vector<std::string>& Matrix<T>::getRowNames() const
{
	return rowNames_;
}

template <typename T>
const std::vector<std::string>& Matrix<T>::getColNames() const
{
	return colNames_;
}

template <typename T> int Matrix<T>::findRow(const std::string& element)
{
	auto res = rowNamesToIndex_.find(element);
	if(res == rowNamesToIndex_.end()) {
		return -1;
	}
	return res->second;
}

template <typename T> int Matrix<T>::findCol(const std::string& element)
{
	auto res = colNamesToIndex_.find(element);
	if(res == colNamesToIndex_.end()) {
		return -1;
	}
	return res->second;
}

template <typename T>
int Matrix<T>::findRow(const std::string& element) const
{
	auto res = rowNamesToIndex_.find(element);
	if(res == rowNamesToIndex_.end()) {
		return -1;
	}
	return res->second;
}

template <typename T>
int Matrix<T>::findCol(const std::string& element) const
{
	auto res = colNamesToIndex_.find(element);
	if(res == colNamesToIndex_.end()) {
		return -1;
	}
	return res->second;
}

template <typename T>
std::vector<T> Matrix<T>::getUniqueRowValues(unsigned int row)
{
	std::set<T> tempSet;
	for(int col = 0; col < colCount_; col++) {
		tempSet.insert(data_[col + row * colCount_]);
	}
	return std::vector<T>(tempSet.begin(), tempSet.end());
}

template <typename T>
std::vector<T> Matrix<T>::getUniqueRowValues(unsigned int row, const T& exclud)
{
	std::set<T> tempSet;
	for(int col = 0; col < colCount_; col++) {
		if(exclud != data_[col + row * colCount_]) {
			tempSet.insert(data_[col + row * colCount_]);
		}
	}
	return std::vector<T>(tempSet.begin(), tempSet.end());
}

template <typename T>
std::vector<T> Matrix<T>::getUniqueColValues(unsigned int col)
{
	std::set<T> tempSet;
	for(int row = 0; row < rowCount_; row++) {
		tempSet.insert(data_[col + row * colCount_]);
	}
	return std::vector<T>(tempSet.begin(), tempSet.end());
}

template <typename T>
std::vector<T> Matrix<T>::getUniqueColValues(unsigned int col, const T& exclud)
{
	std::set<T> tempSet;
	for(int row = 0; row < rowCount_; row++) {
		if(exclud != data_[col + row * colCount_]) {
			tempSet.insert(data_[col + row * colCount_]);
		}
	}
	return std::vector<T>(tempSet.begin(), tempSet.end());
}

template <typename T>
std::vector<T> Matrix<T>::getUniqueRowValues(unsigned int row) const
{
	std::set<T> tempSet;
	for(int col = 0; col < colCount_; col++) {
		tempSet.insert(data_[col + row * colCount_]);
	}
	return std::vector<T>(tempSet.begin(), tempSet.end());
}

template <typename T>
std::vector<T> Matrix<T>::getUniqueRowValues(unsigned int row, const T& exclud) const
{
	std::set<T> tempSet;
	for(int col = 0; col < colCount_; col++) {
		if(exclud != data_[col + row * colCount_]) {
			tempSet.insert(data_[col + row * colCount_]);
		}
	}
	return std::vector<T>(tempSet.begin(), tempSet.end());
}

template <typename T>
std::vector<T> Matrix<T>::getUniqueColValues(unsigned int col) const
{
	std::set<T> tempSet;
	for(int row = 0; row < rowCount_; row++) {
		tempSet.insert(data_[col + row * colCount_]);
	}
	return std::vector<T>(tempSet.begin(), tempSet.end());
}

template <typename T>
std::vector<T> Matrix<T>::getUniqueColValues(unsigned int col, const T& exclud) const
{
	std::set<T> tempSet;
	for(int row = 0; row < rowCount_; row++) {
		if(exclud != data_[col + row * colCount_]) {
			tempSet.insert(data_[col + row * colCount_]);
		}
	}
	return std::vector<T>(tempSet.begin(), tempSet.end());
}

template <typename T>
unsigned int Matrix<T>::countElement(unsigned int colrow, unsigned int number,
                                     const T& t)
{
	// check row
	unsigned int counter = 0;
	if(colrow == 1) {
		for(unsigned int col = 0; col < colCount_; col++) {
			if(data_[col + number * colCount_] == t) {
				counter++;
			}
		}
	}
	// check column
	else if(colrow == 0) {
		for(unsigned int row = 0; row < rowCount_; row++) {
			if(data_[number + row * colCount_] == t) {
				counter++;
			}
		}
	} else
		throw std::invalid_argument("Matrices can not be shrinked");
	return counter;
}

template <typename T>
bool Matrix<T>::containsElement(unsigned int colrow, unsigned int number,const T& t)
{
	// check row
	if(colrow == 1) {
		for(unsigned int col = 0; col < colCount_; col++) {
			if(data_[col + number * colCount_] == t) {
				return true;
			}
		}
		return false;
	} else if(colrow == 0) {
		for(unsigned int row = 0; row < rowCount_; row++) {
			if(data_[number + row * colCount_] == t) {
				return true;
			}
		}
		return false;
	}
	throw std::invalid_argument("First argument must be 0 (col) or 1(row)");
}

template <typename T> bool Matrix<T>::contains(const T& query) const
{
	for(auto res : data_) {
		if(res == query) {
			return true;
		}
	}
	return false;
}

template <typename T>
unsigned int Matrix<T>::countElement(unsigned int colrow, unsigned int number,
                                     const T& t) const
{
	// check row
	unsigned int counter = 0;
	if(colrow == 1) {
		for(unsigned int col = 0; col < colCount_; col++) {
			if(data_[col + number * colCount_] == t) {
				counter++;
			}
		}
	}
	// check column
	else if(colrow == 0) {
		for(unsigned int row = 0; row < rowCount_; row++) {
			if(data_[number + row * colCount_] == t) {
				counter++;
			}
		}
	} else
		throw std::invalid_argument("Matrices can not be shrinked");
	return counter;
}

template <typename T>
bool Matrix<T>::containsElement(unsigned int colrow, unsigned int number,const T& t) const
{
	// check row
	if(colrow == 1) {
		for(unsigned int col = 0; col < colCount_; col++) {
			if(data_[col + number * colCount_] == t) {
				return true;
			}
		}
		return false;
	} else if(colrow == 0) {
		for(unsigned int row = 0; row < rowCount_; row++) {
			if(data_[number + row * colCount_] == t) {
				return true;
			}
		}
		return false;
	}
	throw std::invalid_argument("First argument must be 0 (col) or 1(row)");
}

template <typename T>
void Matrix<T>::readMatrix(const std::string& filename, bool colNames, bool rowNames,
                           const T& initialValue)
{
	std::ifstream input(filename, std::ifstream::in);
	if (!input.good()){
		throw std::invalid_argument("File not found");
	}
	std::string line;
	int numRows = 0;
	int numCols = 0;
	rowNames_.clear();
	colNames_.clear();
	std::vector<std::string> rowNBuffer;
	std::vector<std::string> colNBuffer;
	// Determine number of rows
	while(std::getline(input, line)) {
		numRows++;
	}
	// Determine number of columns
	input.clear();
	input.seekg(0);
	std::stringstream content;
	std::string n;
	std::getline(input, line);
	content << line;
	while(content >> n) {
		numCols++;
	}
	// Adapt matrix
	resize(numCols - int(rowNames), numRows - int(colNames), initialValue);
	// Read data
	input.clear();
	input.seekg(0);
	int row = 0;
	int col = 0;
	
	const auto finder = token_finder(boost::algorithm::is_any_of("\t "),boost::token_compress_on);

	std::string tmp;
	if(colNames) {
		auto it = make_split_iterator(line,finder);
		for(; it != boost::algorithm::split_iterator<std::string::iterator>(); ++it) {
			tmp = boost::copy_range<std::string>(*it);
			if(tmp == "") {
				continue;
			}
			colNBuffer.push_back(tmp);
		}
	}

	while(std::getline(input, line)) {
		auto it = make_split_iterator(line, finder);

		if(rowNames) {
			rowNBuffer.push_back(std::move(boost::copy_range<std::string>(*it)));
			++it;
		}

		for(; it != boost::algorithm::split_iterator<std::string::iterator>(); ++it) {
			tmp = boost::copy_range<std::string>(*it);

			if(tmp == "") {
				continue;
			}

			data_[col + row * colCount_] = boost::lexical_cast<T>(tmp);
			col++;
		}
		
		col = 0;
		row++;
	}
	input.close();
	setRowNames(rowNBuffer);
	setColNames(colNBuffer);
}

template <typename T>
void Matrix<T>::resize(unsigned int colCount, unsigned int rowCount,
                       T initialValue)
{
	if((colCount_ > colCount)or(rowCount_ > rowCount))
		throw std::invalid_argument("Matrices can not be shrinked");
	if(colCount_ < colCount) {
		data_.resize(colCount * rowCount);
		for(int row = rowCount_ - 1; row >= 0; row--)
			for(int col = colCount_ - 1; col >= 0; col--)
				data_[col + row * colCount_ + (colCount - colCount_) * row] =
				    data_[col + row * colCount_];
		for(int row = 0; row < rowCount_; row++)
			for(int col = colCount_; col < colCount; col++)
				data_[col + row * colCount] = initialValue;
		for(int row = rowCount_; row < rowCount; row++)
			for(int col = 0; col < colCount; col++)
				data_[col + row * colCount] = initialValue;
	}
	else{
		if (rowCount_ < rowCount){
			data_.resize(colCount_ * rowCount);
			for(int row = rowCount_; row < rowCount; row++)
				for(int col = 0; col < colCount_; col++)
					data_[col + row * colCount_] = initialValue;
		}
	}
	colCount_ = colCount;
	rowCount_ = rowCount;
}

template <typename T> float Matrix<T>::calculateRowSum(unsigned int row)
{
	float sum = 0;
	if (std::is_arithmetic<T>::value){
		for(int col = 0; col < colCount_; col++)
			sum += (float)data_[col + row * colCount_];
		return sum;
	}
	return -1.0f;
}

template <typename T> float Matrix<T>::calculateColSum(unsigned int col)
{
	float sum = 0;
	if (std::is_arithmetic<T>::value){
		for(int row = 0; row < rowCount_; row++)
			sum += (float)data_[col + row * colCount_];
		return sum;
		}
	return -1.0f;
}

template <typename T>
float Matrix<T>::calculateRowSum(unsigned int row) const
{
	float sum = 0;
	if (std::is_arithmetic<T>::value){
		for(int col = 0; col < colCount_; col++)
			sum += (float)data_[col + row * colCount_];
		return sum;
		}
	return -1.0;
}

template <typename T>
float Matrix<T>::calculateColSum(unsigned int col) const
{
	float sum = 0;
	if (std::is_arithmetic<T>::value){
		for(int row = 0; row < rowCount_; row++)
			sum += (float)data_[col + row * colCount_];
		return sum;
		}
	return -1.0;
}

template <typename T> bool Matrix<T>::hasNACol()
{
	if((findCol("NA") == -1)) {
		return false;
	}
	return true;
}

template <typename T> bool Matrix<T>::hasNARow()
{
	if((findRow("NA") == -1)) {
		return false;
	}
	return true;
}

template <typename T> bool Matrix<T>::hasNACol() const
{
	if((findCol("NA") == -1)) {
		return false;
	}
	return true;
}

template <typename T> bool Matrix<T>::hasNARow() const
{
	if((findRow("NA") == -1)) {
		return false;
	}
	return true;
}

template <typename T> void Matrix<T>::clear()
{
	colCount_ = 0;
	rowCount_ = 0;
	data_.clear();
}
#endif
