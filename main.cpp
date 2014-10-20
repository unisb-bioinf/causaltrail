#include "Matrix.h"
#include <iostream>

int main(int argc, char *argv[]){
	std::cout<<"Hallo Welt"<<std::endl;
	Matrix<int> m = Matrix<int>(2,2);
	m.setData(0,0,0);
	m.setData(1,1,0);
	m.setData(2,0,1);
	m.setData(3,1,1);
	m.print();
	std::cout<<m<<std::endl;
	return 0;
}
