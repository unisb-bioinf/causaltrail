#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <stdexcept>

template<typename T> class Matrix;
template<typename T> std::ostream& operator << (std::ostream&, const Matrix<T>&);

template<typename T>
class Matrix {
	public:
  		Matrix(int rowCount=0, int colCount=0);
	    Matrix(const Matrix& m);
		Matrix& operator=(const Matrix& m);
		~Matrix();
	    inline T& operator () (unsigned int col, unsigned int row);
		inline const T& operator () (unsigned int col, unsigned int row) const;
		friend std::ostream& operator<< <>(std::ostream& os, const Matrix<T>& m);	
		void print();
		void setData(T value, unsigned int col, unsigned int row);
	private:
		void resize(unsigned int rowCount,unsigned int colCount);
		unsigned int rowCount_;
		unsigned int colCount_;
		std::vector<std::string> rowNames_;
		std::vector<std::string> colNames_;
		std::map<std::string, unsigned int> rowNamesToIndex_;
		std::map<std::string, unsigned int> colNamesToIndex_;
		std::vector<T> data_;
};
#endif

template<typename T>
Matrix<T>::Matrix(int rowCount, int colCount)
	:rowCount_(rowCount), colCount_(colCount)
	{
		data_.resize(rowCount_*colCount_,sizeof(T));
	}

template<typename T>
Matrix<T>::~Matrix()
	{
	
	}

template<typename T>
Matrix<T>::Matrix(const Matrix& m)
	:data_(m.data_),rowCount_(m.rowCount_),colCount_(m.colCount_),rowNames_(m.rowNames_), colNames_(m.colNames_),rowNamesToIndex_(m.rowNamesToIndex_),colNamesToIndex_(m.colNamesToIndex_)
	{
		
	}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix& m)
	{
		if (&m == this)
			return *this;
		else
			{
			data_=m.data_;
			rowCount_=m.rowCount_;
			colCount_=m.colCount_;
			rowNames_=m.rowNames_;
			colNames_=m.colNames_;
			rowNamesToIndex_=m.rowNamesToIndex_;
			colNamesToIndex_=m.colNamesToIndex_;
			}
	}

template<typename T>
inline T& Matrix<T>::operator()(unsigned int col, unsigned int row)
	{
		return data_[col+row*colCount_];
	}

template<typename T>
inline const T& Matrix<T>::operator()(unsigned int col, unsigned int row) const
    {
		return data_[col+row*colCount_];
    }

template<typename T>
std::ostream& operator<< (std::ostream& os, const Matrix<T>& m)
	{
		return os<<"Matrix with "<<m.rowCount_<<" rows and "<<m.colCount_<<" columns";
	}

template<typename T>
void Matrix<T>::print()
	{
		std::string temp = "";
		for (int row=0; row < rowCount_; row++){
			for (int col=0; col < colCount_; col++){
				std::stringstream ss;
				ss<<data_[col+row*colCount_];
				temp+=ss.str()+" ";
				ss.clear();
			}
			std::cout<<temp<<std::endl;
			temp="";
		}
	}

template<typename T>
void Matrix<T>::setData(T value, unsigned int col, unsigned int row)
	{
		data_[col+row*colCount_]=value;
	}

template<typename T>
void Matrix<T>::resize(unsigned int rowCount, unsigned int colCount)
	{
		if (colCount_ > colCount)
			throw std::invalid_argument("Matrices can not be shrinked");
		if (colCount_ < colCount)
			{
			data_.resize(rowCount*colCount,sizeof(T));
			for (int row=rowCount-1;row>=0;row--)
				for (int col=0;col<colCount;col++)
					{
						data_[col+row*colCount+(colCount-colCount_)*row]=data_[col+row*colCount];
					}
			colCount_=colCount;
			}
		rowCount_=rowCount;
	}
