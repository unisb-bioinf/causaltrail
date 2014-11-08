#include "Discretiser.h"
#include <algorithm>
/**
 *
 * 
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
Discretiser::Discretiser(std::string datafile, Matrix<int>& obsMatrix, Network& network)
	:originalObservations_(Matrix<std::string>(0,0,"NA")), observations_(obsMatrix), observationsMap_(network.getObservationsMap()), observationsMapR_(network.getObservationsMapR())
	{
	originalObservations_.readMatrix(datafile,false,true,"NA");
	adaptFormat();
	observations_.resize(originalObservations_.getColCount(), originalObservations_.getRowCount(), -1);
	observations_.setRowNames(originalObservations_.getRowNames());
	observations_.setColNames(originalObservations_.getColNames());
}

/**
 *
 * 
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
Discretiser::Discretiser(std::string datafile, std::string filename,  Matrix<int>& obsMatrix, Network& network)
	:originalObservations_(Matrix<std::string>(0,0,"NA")), observations_(obsMatrix), observationsMap_(network.getObservationsMap()), observationsMapR_(network.getObservationsMapR())
	{
	originalObservations_.readMatrix(datafile,false,true,"NA");
	adaptFormat();
	observations_.resize(originalObservations_.getColCount(), originalObservations_.getRowCount(), -1);
	observations_.setRowNames(originalObservations_.getRowNames());
	observations_.setColNames(originalObservations_.getColNames());
	discretise(filename);
}

/**getNumber
 *
 * @param col
 * @param row
 *
 * @return float 
 *
 * Converts the specified entrie of the matrix to a float value, if the value is empty, -1 is returned
 */
float Discretiser::getNumber(unsigned int col, unsigned int row){
	std::stringstream ss;
	std::string temp;
	float value;
	temp=originalObservations_(col,row);
	if (temp=="NA")
		return -1;
	ss<<temp;
	ss>>value;
	return value;
}

/**createSortedVector
 *
 * @param row row to sort
 * 
 * @return sorted vector containing the entries of the specified row
 *
 */
std::vector<float> Discretiser::createSortedVector(unsigned int row){
    std::vector<float> templist;
    for (int col=0;col<originalObservations_.getColCount();col++){
        templist.push_back(getNumber(col,row));
        }   
    std::sort(templist.begin(), templist.end());
	return templist;
}

/**discretiseRow
 *
 * @param
 * @param 
 * 
 * @return void
 *
 * Calls the apropriate method to discretise the specified row
 */
void Discretiser::discretiseRow(unsigned int row, unsigned int method, float threshold){
	switch(method){
		case 0: discretiseCeil(row); break;
		case 1: discretiseFloor(row); break;
		case 2: discretiseRound(row); break;
		case 3: discretiseByAMean(row); break;
		case 4: discretiseByHMean(row); break;
		case 5: discretiseByMedian(row); break;
		case 6: discretiseManually(row,threshold);break;
		case 7: discretiseBracketMedians(row,int(threshold));break;
		case 8: discretisePearsonTukey(row); break;
		case 9: mapNamesToInt(row);break;
		}
	}

/**discretise
 *
 * @param filename name of the file containig the observations
 *
 * @return void
 *
 * Loads the observations, and performs discretisation according to the user input
 *
 */ 
void Discretiser::discretise(std::string controlFile){
	std::ifstream input(controlFile,std::ifstream::in);
	std::string line;
	unsigned int row;
	unsigned int method;
	while(std::getline(input,line)){
		std::stringstream buffer;
		float threshold=0.0;
		buffer<<line;
		buffer>>row>>method>>threshold;
		discretiseRow(row,method,threshold);
		}
	input.close();
}

/**discretiseFloor
 *
 * @param row row to discretise
 *
 * @return void
 *
 * Performs the floor operation on each value in the specified row
 *
 */
void Discretiser::discretiseFloor(unsigned int row){
	for (int col=0;col<originalObservations_.getColCount();col++){
		float value=getNumber(col,row);
		int dvalue=floor(value);
		observations_.setData(dvalue,col,row);
		createNameEntry(dvalue,row);
		}
	}

/**discretiseCeil
 *
 * @param row row to discretise
 *
 * @return void
 *
 * Performs the ceil operation on each value in the specified row
 *
 */
void Discretiser::discretiseCeil(unsigned int row){
	for (int col=0;col<originalObservations_.getColCount();col++){
		float value=getNumber(col,row);
		int dvalue= ceil(value);
		observations_.setData(dvalue,col,row);
		createNameEntry(dvalue,row);
		} 
	}

/**discretiseRound
 *
 * @param row row to discretise
 *
 * @return voi
 *
 * Performs classical rounding on each value in the specified row
 *
 */
void Discretiser::discretiseRound(unsigned int row){
	for (int col=0;col<originalObservations_.getColCount();col++){
		float value=getNumber(col,row);
		int dvalue=round(value);
		observations_.setData(dvalue,col,row);
		createNameEntry(dvalue,row);
		}   
	}  

/**discretiseByAMean
 *
 * @param row row to discretise
 *
 * @return void
 *
 * Divids the data, according to its arithmetic mean, in two groups
 *
 */
void Discretiser::discretiseByAMean(unsigned int row){
	float mean=0;
	for (int col=0;col<originalObservations_.getColCount();col++){
			float value=getNumber(col,row);
			mean+=value;
		}
	mean=mean/originalObservations_.getColCount();
	for (int col=0;col<originalObservations_.getColCount();col++){
			float value=getNumber(col,row);
			if (value > mean){
				observations_.setData(1,col,row);
				createNameEntry(1,row);
			}
			else {
				observations_.setData(0,col,row);
				createNameEntry(0,row);
			}
		}
    }   

/**discretiseByHMean
 *
 * @param row row to discretise
 *
 * @return void
 *
 * Divids the data, according to its harmonic mean, in two groups
 *
 */
void Discretiser::discretiseByHMean(unsigned int row){
    float mean=0;
    for (int col=0;col<originalObservations_.getColCount();col++){
			float value=getNumber(col,row);
            mean+=1.0/value;
        }   
    mean=originalObservations_.getColCount()/mean;
    for (int col=0;col<originalObservations_.getColCount();col++){
            float value=getNumber(col,row);
            if (value > mean){
                observations_.setData(1,col,row);
				createNameEntry(1,row);
            }   
            else {
                observations_.setData(0,col,row);
				createNameEntry(0,row);
            }   
        }   
    }

/**discretiseMedian
 *
 * @param row row to discretise
 *
 * @return void
 *
 * Divids the data, according to its median, in two groups
 *
 */
void Discretiser::discretiseByMedian(unsigned int row){
	std::vector<float> templist=createSortedVector(row);
	float median;
	if (templist.size() % 2 ==0){
		median=templist[templist.size()/2];
		}
	else {
		median=(templist[div(templist.size(),2).quot]+templist[div(templist.size(),2).quot+1])/2.0;
		}
	for (int col=0;col<originalObservations_.getColCount();col++){
		float value=getNumber(col,row);
		if (value > median){
			observations_.setData(1,col,row);
			createNameEntry(1,row);
			}
		else {
			observations_.setData(0,col,row);	
			createNameEntry(0,row);
			}
		}
    }   

/**discretiseManually
 *
 * @param row row to discretise
 * @param threshold
 *
 * @return void
 *
 * Divids the data, accoring to a manually set theshold, in two groups
 *
 */
void Discretiser::discretiseManually(unsigned int row, float threshold){
	for (int col=0;col<originalObservations_.getColCount();col++){
		float value=getNumber(col,row);
		if (value> threshold){
			observations_.setData(1,col,row);
			createNameEntry(1,row);
			}
		else{
			observations_.setData(0,col,row);
			createNameEntry(0,row);
			}	
		}
    }   

/**discretiseBracketMedians
 *
 * @param row row to discretise
 * @param number number of brackets
 *
 * @return void
 *
 * Equally divids the data into a user defined number of brackets
 *
 */
void Discretiser::discretiseBracketMedians(unsigned int row, unsigned int number){
	std::vector<float> templist=createSortedVector(row);
	std::vector<float> borderValues;
	borderValues.reserve(number + 1);
	borderValues.push_back(templist[0]);
	//Calculat Borders
	for (int i=1; i<number;i++){
		borderValues.push_back(templist[div(templist.size(),number).quot*i]);
	}
	borderValues.push_back(templist.back());
	//Fill intervals
	for (int col=0;col<templist.size();col++){
		for (int i=1;i<=number;i++){
			if ((templist[col] >= borderValues[i-1]) and (templist[col]<borderValues[i])){
				observations_.setData(i-1, col, row);
				createNameEntry(i-1, row);
				break;
			}
		}
	}
}   

/**discretisePearsonTukey
 *
 * @param row row to discretise
 *
 * @return void
 *
 * Performs the Person Tukey discretisation method. Data
 *
 */
void Discretiser::discretisePearsonTukey(unsigned int row){
	std::vector<float> templist=createSortedVector(row);
	std::vector<float> borderValues = {
		//Calculate borders
		templist[0],
		templist[ceil(0.185*templist.size())],
		templist[ceil(0.815*templist.size())],
		templist.back()
	};
	//Fill intervals
	for (int col=0;col<templist.size();col++){
		for (int i=1;i<=3;i++){
			float value=templist[col];
			if ((value >= borderValues[i-1]) and (value<borderValues[i])){
				observations_.setData(i-1,col,row);
				createNameEntry(i-1,row);
				break;
			}   
		} 	
	}
}   

/**mapNamesToInt
 *
 * @return void
 *
 * maps values of typ string to integer values to allow storing in the observation_ matrix
 */
void Discretiser::mapNamesToInt(unsigned int row){
	float index=0;
	std::vector<std::string> uniqueValues = originalObservations_.getUniqueRowValues(row);
	for (auto value : uniqueValues){
		if ((value != "NA")){
			observationsMap_[value]=index;
			observationsMapR_[std::make_pair(index,row)]=value;
			index++;	
		}
		else{
			observationsMap_[value]=-1;
			observationsMapR_[std::make_pair(-1,row)]=value;
		}
	}
	for (int col=0;col<originalObservations_.getColCount();col++){
		observations_.setData(observationsMap_[originalObservations_(col,row)],col,row);
	}
}

/**createNameEntry
 *
 * @param value
 * @param row
 * 
 * @return void
 * 
 * Creates an name entry for the discretised value
 */
void Discretiser::createNameEntry(int value, unsigned int row){
	std::stringstream ss;
	std::string ssvalue;
	ss<<value;
	ss>>ssvalue;
	observationsMap_[ssvalue]=value;
	observationsMapR_[std::make_pair(value,row)]=ssvalue;
}

/**adaptFormat
 *
 * @param value
 * @param row
 * 
 * @return void
 * 
 * Unifies the format of the original observations matrix
 */
void Discretiser::adaptFormat(){
	for (unsigned int col=0; col<originalObservations_.getColCount(); col++){
		for (unsigned int row=0; row<originalObservations_.getRowCount(); row++){
			const std::string value=originalObservations_(col,row);
			if ((value == "NA") or (value =="na") or (value=="-") or (value == "/"))
				originalObservations_.setData("NA",col,row);
		}
	}
}
