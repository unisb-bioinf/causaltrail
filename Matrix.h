#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <unordered_map>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

template<typename T> class Matrix;
template<typename T> std::ostream& operator << (std::ostream&, const Matrix<T>&);

template<typename T>
class Matrix {
	public:
  		Matrix(int colCount=0, int rowCount=0, T initialValue=NULL);
	    inline T& operator () (unsigned int col, unsigned int row);
		inline const T& operator () (unsigned int col, unsigned int row) const;
		friend std::ostream& operator<< <>(std::ostream& os, const Matrix<T>& m);	
		void setData(T value, unsigned int col, unsigned int row);
		void setRowNames(std::vector<std::string> names);
		void setColNames(std::vector<std::string> names);
		unsigned int getRowCount();
		unsigned int getColCount();
		T& getValueByNames(const std::string& colName, const std::string& rowName);
		std::vector<std::string>& getRowNames();
		std::vector<std::string>& getRowNames() const;
		std::vector<std::string>& getColNames();
		std::vector<std::string>& getColNames() const;
		int findRow(const std::string& element);
		int findCol(const std::string& element);
		float calculateRowSum(unsigned int row);
		float calculateColSum(unsigned int col);
		std::vector<T> getUniqueRowValues(unsigned int row);
		std::vector<T> getUniqueRowValues(unsigned int row, T exclud);
		std::vector<T> getUniqueColValues(unsigned int col, T exclud);
		std::vector<T> getUniqueColValues(unsigned int col);
		bool contains(const T& query);
		void readMatrix(std::string& filename, bool colNames, bool rowNames,T initialValue);
		unsigned int countElement(unsigned int colrow,unsigned int number, T t);
		bool containsElement(unsigned int colrow, unsigned int number, T t);
		void resize(unsigned int colCount,unsigned int rowCount, T initalValue);
		bool hasNACol();
		bool hasNARow();
		void clear();
		private:
		unsigned int rowCount_;
		unsigned int colCount_;
		std::vector<std::string> rowNames_;
		std::vector<std::string> colNames_;
		std::unordered_map<std::string, unsigned int> rowNamesToIndex_;
		std::unordered_map<std::string, unsigned int> colNamesToIndex_;
		std::vector<T> data_;
};
#endif

/**Detailed Constructor
 *
 * @param colCount number of columns
 * @param rowCount number of rows
 * @param initialValue initial value for all points in the matrix
 *
 * @return A Matrix Object
 * 
 * Creates a Matrix with the specified number of columns and rows
 */
template<typename T>
Matrix<T>::Matrix(int colCount, int rowCount, T initialValue)
	:rowCount_(rowCount), colCount_(colCount), data_(rowCount*colCount, initialValue)
	{
	}

/**Operator()
 *
 * @param col column number
 * @param row row number
 *
 * @return Value at position Matrix[col,row]
 * 
 * This operator returns the matrix value at the specified position
 */
template<typename T>
inline T& Matrix<T>::operator()(unsigned int col, unsigned int row){
	return data_[col+row*colCount_];
	}

/**Operator() const
 *
 * @param col column number
 * @param row row number
 *
 * @return Value at position Matrix[col,row]
 * 
 * This operator returns the matrix value at the specified position
 *
 */
template<typename T>
inline const T& Matrix<T>::operator()(unsigned int col, unsigned int row) const{
	return data_[col+row*colCount_];
    }

/**Title
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
template<typename T>
T& Matrix<T>::getValueByNames(const std::string& colName, const std::string& rowName){
	int col=findCol(colName);
	int row=findRow(rowName);
	if ((row==-1) or (col==-1)){
		throw std::invalid_argument("Specified elements not found");
		}
	else return data_[col+row*colCount_];
		}

/**Operator <<
 *
 * @param os Reference to an ostream
 * @param m Reference to a matrix object
 *
 * @return ostream containing the content of the matrix
 * 
 * This operator adds the content of the matrix to an ostream. 
 * Column and Rownames are included if they are  available.
 *
 */
template<typename T>
std::ostream& operator<< (std::ostream& os, const Matrix<T>& m)
{	//Write Column Names
	os<<"\t";
	for (std::vector<std::string>::const_iterator it = m.colNames_.begin(); it != m.colNames_.end(); ++it)
		os<<*it<<"\t";
	os<<"\n";
	//Write Content & Row Names
	for (int row=0; row < m.rowCount_; row++){
		if (row<m.rowNames_.size())
			os<<m.rowNames_[row]<<"\t";
		else os<<"\t";
		for (int col=0; col < m.colCount_; col++)
			os<<m.data_[col+row*m.colCount_]<<"\t";	
			if (row < m.rowCount_-1)
				os<<"\n";
		}
	return os;
}

/**setData
 *
 * @param value, value to be added into the matrix
 * @param col, destination column
 * @param row, destination row
 *
 * @return void
 * 
 * This method stores a value in the matrix at the specified position
 *
 * @throw Exception
 */
template<typename T>
void Matrix<T>::setData(T value, unsigned int col, unsigned int row){
	data_[col+row*colCount_]=value;
	}

/**setRowNames
 *
 * @param names vector containing row names
 *
 * @return void
 * 
 * This method stors the rownames of the matrix. It also generates a mapping between
 * rownames and index
 *
 */
template<typename T>
void Matrix<T>::setRowNames(const std::vector<std::string> names){
	rowNames_=names;
	unsigned int counter=0;
	for (const auto& n : names){
		rowNamesToIndex_[n]=counter;
		counter++;
		}
	}

/**setColNames
 *
 * @param names vector containing column names
 *
 * @return void
 * 
 * This method stors the rownames of the matrix. It also generates a mapping between
 * columnames and index
 *
 */
template<typename T>
void Matrix<T>::setColNames(const std::vector<std::string> names){
	colNames_=names;
	unsigned int counter=0;
	for (const auto& n : names){   
		colNamesToIndex_[n]=counter;
		counter++;
		}
	}

/**getRowCount
 *
 * @return Number of rows
 * 
 * This function returns the number of rows of the matrix
 */
template<typename T>
unsigned int Matrix<T>::getRowCount(){
	return rowCount_;
	}

/**getColCount
 *
 * @return Number of columns
 *
 * This function returns the number of columns
 */
template<typename T>
unsigned int Matrix<T>::getColCount(){
	return colCount_;
	}

/**getRowNames
 *
 * @return rowNames
 * 
 * Returns a reference to the row names of the matrix
 */
template<typename T>
std::vector<std::string>& Matrix<T>::getRowNames(){
	return rowNames_;
	}

/**getColNames
 *
 * @return column names
 * 
 * Returns a reference to the column names of the matrix
 */
template<typename T>
std::vector<std::string>& Matrix<T>::getColNames(){
	return colNames_;
	}

/**findRow
 *
 * @param element query name of the row
 *
 * @return row index containing the query value, -1 if the matrix does not contain such a row
 * 
 * This function returns the index of the row, which matches the given row name
 */
template<typename T>
int Matrix<T>::findRow(const std::string& element){
	auto res = rowNamesToIndex_.find(element);
	if(res == rowNamesToIndex_.end()) {
		return -1;
		}
	return res->second;
	}

/**findCol
 *
 * @param element query name of the col
 *
 * @return col index containing the query value, -1 if the matrix does not contain such a col
 * 
 * This function returns the index of the col, which matches the given col name
 */
template<typename T>
int Matrix<T>::findCol(const std::string& element){
	auto res = colNamesToIndex_.find(element);
	if (res == colNamesToIndex_.end()){
		return -1;
		}
	return res->second;
	}

/**getUniqueRowValues
 *
 * @param row index
 *
 * @return vector containing the unique elements stored in the query row of the matrix
 * 
 * This function returns the unique elements which are stored in the specified row of the matrix
 */
template<typename T>
std::vector<T> Matrix<T>::getUniqueRowValues(unsigned int row){
	std::set<T> tempSet;
	for (int col=0;col<colCount_;col++)	{
		tempSet.insert(data_[col+row*colCount_]);
		}	
	return std::vector<T> (tempSet.begin(),tempSet.end());
	}

/**getUniqueRowValues
 *
 * @param row index
 * @param exclud
 *
 * @return vector containing the unique elements stored in the query row of the matrix
 * 
 * This function returns the unique elements which are stored in the specified row of the matrix
 */
template<typename T>
std::vector<T> Matrix<T>::getUniqueRowValues(unsigned int row, T exclud){
    std::set<T> tempSet;
    for (int col=0;col<colCount_;col++) {
		if (exclud!=data_[col+row*colCount_]){
	        tempSet.insert(data_[col+row*colCount_]);
			}
        }   
    return std::vector<T> (tempSet.begin(),tempSet.end());
    }

/**getUniqueColValues
 *
 * @param col index
 *
 * @return vector containing the unique elements stored in the query col of the matrix
 * 
 * This function returns the unique elements which are stored in the specified col of the matrix
 */
template<typename T>
std::vector<T> Matrix<T>::getUniqueColValues(unsigned int col){	
	std::set<T> tempSet;
	for (int row=0;row<rowCount_;row++){
		tempSet.insert(data_[col+row*colCount_]);
		}
	return std::vector<T> (tempSet.begin(),tempSet.end());
	}

/**getUniqueColValues
 *
 * @param col index
 *
 * @return vector containing the unique elements stored in the query col of the matrix
 * 
 * This function returns the unique elements which are stored in the specified col of the matrix
 */
template<typename T>
std::vector<T> Matrix<T>::getUniqueColValues(unsigned int col, T exclud){
    std::set<T> tempSet;
    for (int row=0;row<rowCount_;row++){
		if (exclud!=data_[col+row*colCount_]){	
	    	tempSet.insert(data_[col+row*colCount_]);
			}
        }
    return std::vector<T> (tempSet.begin(),tempSet.end());
    }


/**contains
 *
 * @param query query value 
 *
 * @return true, if the matrix contains the query element, false otherwise 
 *
 * This function checks, whether the query element is stored in the matrix or not
 */
template<typename T>
bool Matrix<T>::contains(const T& query){
	for (auto res : data_){
		if (res == query){
			return true;
			}
		}
	return false;
	}

/**countElement
 *
 * @param colrow control variable controls whether rows or columns should be analysed 1=row, 0=col
 * @param number index of the row or column which should be analysed
 * @param query value
 *
 * @return the number of occurence of element t in the specified column or row of the matrix
 * 
 * This function counts how often a certain element is stored in a specified column or row of the matrix
 */
template<typename T>
unsigned int Matrix<T>::countElement(unsigned int colrow,unsigned int number, T t){
	//check row
	unsigned int counter =0;
	if (colrow==1){
		for (unsigned int col=0;col<colCount_;col++){
			if (data_[col+number*colCount_]==t){
				counter++;
			}
		}
	}
	//check column
	else if (colrow==0){
			for (unsigned int row=0;row<rowCount_;row++){
				if (data_[number+row*colCount_]==t){
					counter++;
				}
			}
		}
	else throw std::invalid_argument("Matrices can not be shrinked");
	return counter;
	}

/**countainsElement
 *
 * @param colrow control variable controls whether rows or columns should be analysed 1=row, 0=col
 * @param number index of the row or column which should be analysed
 * @param query value
 *
 * @return 0 if Element is not contained in the specified row or colum, 1 otherwise
 *
 * The method checks wheter a certain element is contained in a row or a column
 */
template<typename T>
bool Matrix<T>::containsElement(unsigned int colrow, unsigned int number, T t){
	//check row
	if (colrow==1){
		for (unsigned int col = 0; col<colCount_;col++){
			if (data_[col+number*colCount_]==t){
				return true;
				}
			}
		return false;
		}
	else if (colrow==0){
			for(unsigned int row=0;row<rowCount_;row++){
				if (data_[number+row*colCount_]==t){
					return true;
					}
				}
			return false;
			}
		throw std::invalid_argument("First argument must be 0 (col) or 1(row)");
	}



/**readMatrix
 * 
 * @param filename file that should be read
 * @param rowNames flag indicating whether the matrix contains rowNames
 * @param colNames flag indicating whether the matrix contains colNames
 * @param initialValue value that should be used for initilization
 * 
 * @return void
 * 
 * This methods reads a tab or space delimited file containing a matrix.
 */
template<typename T>
void Matrix<T>::readMatrix(std::string& filename, bool colNames, bool rowNames, T initialValue){
	std::ifstream input(filename, std::ifstream::in);
	std::string line;
	int numRows=0;
	int numCols=0;
	rowNames_.clear();
	colNames_.clear();
	std::vector<std::string> rowNBuffer;
	std::vector<std::string> colNBuffer;
	//Determine number of rows
	while(std::getline(input,line))	{
		numRows++;
	}
	//Determine number of columns
	input.clear();
	input.seekg(0);
	std::stringstream content;
	std::string n;
	std::getline(input,line);
	content<<line;
	while(content>>n){
		numCols++;
	}
	//Adapt matrix
	resize(numCols-int(rowNames),numRows-int(colNames),initialValue);
	//Read data
	input.clear();
	input.seekg(0);
	T t;
	int row=0;
	int col=0;
	if (colNames){
		std::getline(input,line);
		std::stringstream colNameBuffer;
		colNameBuffer<<line;
		colNameBuffer>>n;
		while(colNameBuffer>>n){
			colNBuffer.push_back(n);
		}
	}
	while(std::getline(input,line)){
		std::stringstream buffer;
		buffer<<line;
		if (rowNames){
			buffer>>n;
			rowNBuffer.push_back(n);
		}
		while(buffer>>t){
			data_[col+row*colCount_]=t;
			col++;
		}
		col=0;
		row++;
	}	
	input.close();
	setRowNames(rowNBuffer);
	setColNames(colNBuffer);
}

/**resize
 *
 * @param colCount new number of columns
 * @param rowCount new number of rows
 * @param initialValue value that should be used for initialisation
 *
 * @return void
 * 
 * This function adapts the size of the matrix. A matrix can not be shrinked, only enlarged.
 * Existing elements will stay at their original matrix position.
 *
 * @throw Exception invalid_argument(If the original number of colums or rows is smaller than the new one)
 */
template<typename T>
void Matrix<T>::resize(unsigned int colCount, unsigned int rowCount,T initialValue){
	if ((colCount_ > colCount) or (rowCount_ > rowCount))
		throw std::invalid_argument("Matrices can not be shrinked");
	if (colCount_ < colCount){
		data_.resize(colCount*rowCount); 	
		for (int row=rowCount_-1;row>=0;row--)
			for (int col=colCount_-1;col>=0;col--)
					data_[col+row*colCount_+(colCount-colCount_)*row]=data_[col+row*colCount_];
		for (int row=0;row<rowCount_;row++)
			for (int col=colCount_;col<colCount;col++)
					data_[col+row*colCount]=initialValue;
		for (int row=rowCount_;row<rowCount;row++)
			for (int col=0;col<colCount;col++)
					data_[col+row*colCount]=initialValue;
		}
	colCount_=colCount;
	rowCount_=rowCount;
	}

/**calculateRowSum
 *
 * @param row
 * 
 * @return 
 *
 *
 */
template<typename T>
float Matrix<T>::calculateRowSum(unsigned int row){
	float sum = 0;
	for (int col=0;col<colCount_;col++)
		sum+=(float)data_[col+row*colCount_];
	return sum;
	}

/**calculateColSum
 *
 * @param col
 * 
 * @return
 * 
 *
 */
template<typename T>
float Matrix<T>::calculateColSum(unsigned int col){
	float sum = 0;
	for (int row=0;row<rowCount_;row++)
		sum+=(float)data_[col+row*colCount_];
	return sum;	
	}

/**hasNACol
 *
 * return true if NA (or variants) are columnNames
 */
template<typename T>
bool Matrix<T>::hasNACol(){
	if ((findCol("NA") ==-1) and (findCol("na")==-1) and (findCol("-")==-1) and (findCol("-1")==-1)){
		return false;
		}
	return true;
}

/**hasNARow
 *
 *return true if NA (or variants) are rowNames
 */
template<typename T>
bool Matrix<T>::hasNARow(){
	if ((findRow("NA") ==-1) and (findRow("na")==-1) and (findRow("-")==-1) and (findRow("-1")==-1)){
		return false;
		}   
    return true;
	}
/**clear
 *
 * @return void
 *
 * Reset operation for the matrix.
 */
template<typename T>
void Matrix<T>::clear(){
	colCount_=0;
	rowCount_=0;
	data_.clear();
	}
