#include "Controller.h"
#include <cmath>
#include <algorithm>

/**Constructor
*
* @return A Controller Object
* 
*/
Controller::Controller()
	:network_(Network()),originalObservations_(Matrix<std::string>(0,0,"NA")),observations_(Matrix<int>(0,0,0))
	{
	}

/**loadNetwork
 *
 * @param filename name of the file containing the (partial) network structure
 *
 * @return void
 *
 */
void Controller::loadNetwork(std::string filename){
	network_.readNetwork(filename);
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
float Controller::getNumber(unsigned int col, unsigned int row){
	std::stringstream ss;
	std::string temp;
	float value;
	temp=originalObservations_(col,row);
	if ((temp=="NA") or (temp=="na") or (temp=="-"))
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
std::vector<float> Controller::createSortedVector(unsigned int row){
    std::vector<float> templist;
    for (int col=0;col<originalObservations_.getColCount();col++){
        templist.push_back(getNumber(col,row));
        }   
    std::sort (templist.begin(), templist.end());
	return templist;
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
void Controller::discretise(std::string filename){
	originalObservations_.readMatrix(filename,false,true,"NA");
	observations_.resize(originalObservations_.getColCount(), originalObservations_.getRowCount(), -1);
	observations_.setRowNames(originalObservations_.getRowNames());
	observations_.setColNames(originalObservations_.getColNames());
	mapNamesToInt(0);
	mapNamesToInt(1);
	mapNamesToInt(2);
	mapNamesToInt(3);
	mapNamesToInt(4);
//	std::cout<<observations_;
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
void Controller::discretiseFloor(unsigned int row){
	for (int col=0;col<originalObservations_.getColCount();col++){
		float value=getNumber(col,row);
		observations_.setData(floor(value),col,row);
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
void Controller::discretiseCeil(unsigned int row){
	for (int col=0;col<originalObservations_.getColCount();col++){
		float value=getNumber(col,row);
		observations_.setData(ceil(value),col,row);
		} 
	}

/**discretiseRound
 *
 * @param row row to discretise
 *
 * @return voi
 *
 * Performs classical rounding on each each in the specified row
 *
 */
void Controller::discretiseRound(unsigned int row){
	for (int col=0;col<originalObservations_.getColCount();col++){
		float value=getNumber(col,row);
		observations_.setData(round(value),col,row);
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
void Controller::discretiseByAMean(unsigned int row){
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
			}
			else {
				observations_.setData(0,col,row);
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
void Controller::discretiseByHMean(unsigned int row){
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
            }   
            else {
                observations_.setData(0,col,row);
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
void Controller::discretiseByMedian(unsigned int row){
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
			}
		else {
			observations_.setData(0,col,row);	
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
void Controller::discretiseManually(unsigned int row, float threshold){
	for (int col=0;col<originalObservations_.getColCount();col++){
		float value=getNumber(col,row);
		if (value> threshold){
			observations_.setData(1,col,row);
			}
		else{
			observations_.setData(0,col,row);
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
void Controller::discretiseBracketMedians(unsigned int row, unsigned int number){
	std::vector<float> templist=createSortedVector(row);
	std::vector<float> borderValues;
	float value;
	borderValues.push_back(templist[0]);
	//Calculat Borders
	for (int i=1; i<number;i++){
		borderValues.push_back(templist[div(templist.size(),number).quot*i]);
	}
	borderValues.push_back(templist[templist.size()-1]);
	//Fill intervals
	for (int col=0;col<templist.size();col++){
		for (int i=1;i<=number;i++){
			value=templist[col];
			if ((value >= borderValues[i-1]) and (value<borderValues[i])){
				observations_.setData(i-1,col,row);
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
void Controller::discretisePearsonTukey(unsigned int row){
	std::vector<float> templist=createSortedVector(row);
	std::vector<float> borderValues;
	float value;
	//Calculate borders
	borderValues.push_back(templist[0]);
	borderValues.push_back(templist[ceil(0.185*templist.size())]);
	borderValues.push_back(templist[ceil(0.815*templist.size())]);
	borderValues.push_back(templist[templist.size()-1]);
	//Fill intervals
	for (int col=0;col<templist.size();col++){
		for (int i=1;i<=3;i++){
			value=templist[col];
			if ((value >= borderValues[i-1]) and (value<borderValues[i])){
				observations_.setData(i-1,col,row);
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
void Controller::mapNamesToInt(unsigned int row){
	float index=0;
	std::vector<std::string> uniqueValues = originalObservations_.getUniqueRowValues(row);
	for (auto value : uniqueValues){
		if ((value != "NA") and (value != "na") and (value != "-")){
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

/**distributeObservations
 *
 * @return void
 *
 * Assigns the discretised observations to the nodes in the network according to their names and the network structure
 */
void Controller::distributeObservations(){
	unsigned int parentCombinations;
	unsigned int obsRow;
	unsigned int nodeValueCounts;
	unsigned int parentRow;
	std::vector<int> uniqueNodeValues;
	std::vector<int> uniqueParentValues;
	std::vector<std::string> nodeValueNames;
	std::vector<std::string> nodeValueNamesProb;
	std::vector<std::vector<int>> parentNameInts;
	std::vector<std::string> parentNames;
	std::unordered_map<int,std::vector<int>> parentValues;
	//Generating matrices
	for (auto& n:network_.getNodes()){
		nodeValueNames.clear();
		nodeValueNamesProb.clear();
		parentNames.clear();
		parentNameInts.clear();
		obsRow = observations_.findRow(n.getName());
		auto parentIDs =  network_.getParents(n.getID());
		parentCombinations=1;
		//Number of columns
		uniqueNodeValues=observations_.getUniqueRowValues(obsRow);
		nodeValueCounts=uniqueNodeValues.size();
		//Column names
		for (auto value: uniqueNodeValues){
			nodeValueNames.push_back(observationsMapR_[std::make_pair(value,obsRow)]);
			if (value != -1){
				nodeValueNamesProb.push_back(observationsMapR_[std::make_pair(value,obsRow)]);
				}
			}
		for (auto parentID : parentIDs){
			//Number of rows
			parentRow=observations_.findRow(network_.getNode(parentID).getName());
			uniqueParentValues=observations_.getUniqueRowValues(parentRow);
			parentCombinations*=(uniqueParentValues.size()-(int)observations_.containsElement(1,parentRow,-1));
			//Row names
			parentValues[parentID]=uniqueParentValues;
			}
		if (parentIDs.size() > 1){
			Combinations<int> comb = Combinations<int>(parentIDs,parentValues);
			comb.createCombinations(0);
			parentNameInts=comb.getResult();
			for (auto vec : parentNameInts){
				std::string temp="";
				for (unsigned int key=0; key<parentIDs.size();key++){
					parentRow=observations_.findRow(network_.getNode(parentIDs[key]).getName());
					temp+=observationsMapR_[std::make_pair(vec[key],parentRow)];
					}
				parentNames.push_back(temp);
				}
			}
		else if (parentIDs.size()==1){
				parentRow=observations_.findRow(network_.getNode(parentIDs[0]).getName());
				for (auto v : uniqueParentValues){
					parentNames.push_back(observationsMapR_[std::make_pair(v,parentRow)]);				
					}
				}	
		else {
			parentNames.push_back("1");
			}
		//Generating suitable objects
		Matrix<int> obsMatrix = Matrix<int>(nodeValueCounts,parentCombinations,0);
		obsMatrix.setColNames(nodeValueNames);
		obsMatrix.setRowNames(parentNames);
		Matrix<float> probMatrix = Matrix<float>(nodeValueCounts-(int)observations_.containsElement(1,obsRow,-1),parentCombinations,0.0);
		probMatrix.setColNames(nodeValueNamesProb);	
		probMatrix.setRowNames(parentNames);
		//Count observations
		for (unsigned int sample = 0; sample<observations_.getColCount();sample++){
			std::string rowName="";
			std::string colName=originalObservations_(sample,originalObservations_.findRow(n.getName()));
			for (auto parentID:parentIDs){
				rowName+=originalObservations_(sample,originalObservations_.findRow(network_.getNode(parentID).getName()));
				}
			if (rowName==""){
				rowName="1";
				}
			obsMatrix.setData(obsMatrix.getValueByNames(colName,rowName)+1,obsMatrix.findCol(colName),obsMatrix.findRow(rowName));
			}
		n.setObservations(obsMatrix);
		n.setProbability(probMatrix);
		std::cout<<n;
		}
	}
