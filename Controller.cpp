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
void Controller::loadNetwork(std::string networkfile){
	network_.readNetwork(networkfile);
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

/**discretiseRow
 *
 * @param
 * @param 
 * 
 * @return void
 *
 * Calls the apropriate method to discretise the specified row
 */
void Controller::discretiseRow(unsigned int row, unsigned int method, float threshold){
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
void Controller::discretise(std::string datafile, std::string controlFile){
	originalObservations_.readMatrix(datafile,false,true,"NA");
	observations_.resize(originalObservations_.getColCount(), originalObservations_.getRowCount(), -1);
	observations_.setRowNames(originalObservations_.getRowNames());
	observations_.setColNames(originalObservations_.getColNames());
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
void Controller::discretiseFloor(unsigned int row){
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
void Controller::discretiseCeil(unsigned int row){
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
 * Performs classical rounding on each each in the specified row
 *
 */
void Controller::discretiseRound(unsigned int row){
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
void Controller::discretiseManually(unsigned int row, float threshold){
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
				createNameEntry(i-1,row);
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
	//Calculate borders
	borderValues.push_back(templist[0]);
	borderValues.push_back(templist[ceil(0.185*templist.size())]);
	borderValues.push_back(templist[ceil(0.815*templist.size())]);
	borderValues.push_back(templist[templist.size()-1]);
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

/**createNameEntry
 *
 * @param value
 * @param row
 * 
 * @return void
 * 
 * Creates an name entry for the discretised value
 */
void Controller::createNameEntry(int value, unsigned int row){
	std::stringstream ss;
	std::string ssvalue;
	ss<<value;
	ss>>ssvalue;
	observationsMap_[ssvalue]=value;
	observationsMapR_[std::make_pair(value,row)]=ssvalue;
	}

/**tempNodeInfo
 *
 * Struct to store information used in the distributeObservations method
 */

struct tempNodeInfo {
	std::vector<std::string> nodeValueNames;
	std::vector<std::string> nodeValueNamesProb;
	std::vector<std::vector<int>> parentNameInts;
	std::vector<std::string> parentNames;
	std::vector<unsigned int> parentIDs;
	std::vector<int> uniqueValues;
	std::vector<int> uniqueValuesExcludingNA;
	int parentCombinations;
	int obsRow;
};

/**distributeObservations
 *
 * @return void
 *
 * Assigns the discretised observations to the nodes in the network according to their names and the network structure
 */
void Controller::distributeObservations(){
	std::unordered_map<int,std::vector<int>> uniqueValues;
	std::unordered_map<int,std::vector<int>> uniqueValuesExcludingNA;
	std::unordered_map<int,int> observationRow;
	//ComputeUniqueNodeValues
	for (auto& n:network_.getNodes()){
		int obsRow=observations_.findRow(n.getName());
		observationRow[n.getID()]=obsRow;
		uniqueValues[n.getID()]=observations_.getUniqueRowValues(obsRow);
		uniqueValuesExcludingNA[n.getID()]=observations_.getUniqueRowValues(obsRow,-1);
	}
	
	//Generating matrices
	for (auto& n:network_.getNodes()){
		tempNodeInfo tni;
		tni.parentIDs = network_.getParents(n);
		tni.parentCombinations=1;
		tni.obsRow=observationRow[n.getID()];
		tni.uniqueValues=uniqueValues[n.getID()];
		tni.uniqueValuesExcludingNA=uniqueValuesExcludingNA[n.getID()];
		for (auto value: tni.uniqueValues){
			tni.nodeValueNames.push_back(observationsMapR_[std::make_pair(value,tni.obsRow)]); 		//TODO: Add security
			if (value != -1){
				tni.nodeValueNamesProb.push_back(observationsMapR_[std::make_pair(value,tni.obsRow)]);
				}
			}
		//Function PARENTVALUES
		for (auto parentID : tni.parentIDs){
			//Number of rows
			tni.parentCombinations*=uniqueValuesExcludingNA[parentID].size();
			}
		if (tni.parentIDs.size() > 1){
			Combinations<int> comb = Combinations<int>(tni.parentIDs,uniqueValuesExcludingNA);
			comb.createCombinations(0);
			tni.parentNameInts=comb.getResult();
		//FUNCTION PARENTNAMES
			for (auto vec : tni.parentNameInts){
				std::string temp="";
				for (unsigned int key=0; key<tni.parentIDs.size();key++){
					int parentRow=observationRow[tni.parentIDs[key]];
					temp=temp+observationsMapR_[std::make_pair(vec[key],parentRow)]+",";
				}
				temp.erase(temp.end()-1);
				tni.parentNames.push_back(temp);
			}
		}
		else if (tni.parentIDs.size()==1){
			//FUNCTION PARENTNAMES SIMPLE
				int parentRow=observationRow[tni.parentIDs[0]];
				for (auto v : uniqueValuesExcludingNA[tni.parentIDs[0]]){
					tni.parentNames.push_back(observationsMapR_[std::make_pair(v,parentRow)]);				
					}
				}	
		else {
			tni.parentNames.push_back("1");
			}

		//Generating suitable objects
		Matrix<int> obsMatrix = Matrix<int>(tni.uniqueValues.size(),tni.parentCombinations,0);
		obsMatrix.setColNames(tni.nodeValueNames);
		obsMatrix.setRowNames(tni.parentNames);
		std::cout<<obsMatrix<<std::endl;
		Matrix<float> probMatrix = Matrix<float>(tni.uniqueValuesExcludingNA.size(),tni.parentCombinations,0.0);
		probMatrix.setColNames(tni.nodeValueNamesProb);	
		probMatrix.setRowNames(tni.parentNames);
		std::cout<<probMatrix<<std::endl;	
	
		//Count observations
		for (unsigned int sample = 0; sample<observations_.getColCount();sample++){
			std::string rowName="";
			std::string colName=observationsMapR_[std::make_pair(observations_(sample,tni.obsRow),tni.obsRow)];
			for (auto parentID : tni.parentIDs){
				int row=observationRow[parentID];	
				rowName=rowName+observationsMapR_[std::make_pair(observations_(sample,row),row)]+",";
			}
			if (rowName==""){
				rowName="1";
			}
			if ((rowName.find("NA")==std::string::npos) and (rowName.find("na")==std::string::npos) and (rowName.find("-")==std::string::npos)){
				if (rowName != "1"){
					rowName.erase(rowName.end()-1);
				}
				obsMatrix.setData(obsMatrix.getValueByNames(colName,rowName)+1,obsMatrix.findCol(colName),obsMatrix.findRow(rowName));
			}
		}
	n.setObservations(obsMatrix);
	n.setObservationBackup(obsMatrix);
	n.setProbability(probMatrix);
	n.createBackup();
	}
}


/**split
 *
 * @param strint to split
 * @param character to split at
 *
 * @return a vector of substrings
 *
 * splits the given string at the specified delimeters and returns a substring
 */
std::vector<std::string> Controller::split(std::string& str, char delim){
    std::stringstream ss(str);
    std::string item;
    std::vector<std::string> result;
    while(std::getline(ss,item,delim)){
        result.push_back(item);
        }
    return result;
    }

/**computeTotalProbability
 *
 * @param identifier of the query node
 * @param value of the query node
 *
 * @return totalProbability for the given value
 * 
 * Recursively calculates the total probability for a given query
 */
float Controller::computeTotalProbability(unsigned int nodeID, std::string value){ 
    //GetNode
    Node& n = network_.getNode(nodeID);
    //Get Parents
    auto parentIDs= network_.getParents(n);
    Matrix<float> probMatrix = n.getProbabilityMatrix();
    //Check Existens
    if (parentIDs.size()!=0){
    //Yes -> Call recursively for all parent values
        float result = 0;
        for (int row=0;row<probMatrix.getRowCount();row++){
            auto values = split(probMatrix.getRowNames()[row],',');
            float temp=1.0;
            for (int index=0; index<values.size();index++){
                temp*=computeTotalProbability(parentIDs[index],values[index]);
                }   
            result+=(temp*probMatrix(probMatrix.findCol(value),row));
            }   
        return result;
        }   
    //No -> return value
    else {
        return probMatrix(probMatrix.findCol(value),0);
        }   
    }  


/**performEM
 *
 * @return void
 *
 * Calls the appropiate methods to train the model.
 */

void Controller::performEM(){
	float difference=1.0;
	int runs=0;
	//Check completness of the data
	if (observations_.contains(-1)){
		//Initialize and iterate E/M-Phase
		initialise(0);
		while ((difference > 0.001) and (runs < 10000)){
			ePhase();
			difference=mPhase();
			std::cout<<difference<<std::endl;
			runs++;
			}
		std::cout<<"Runs: "<<runs<<std::endl;
		std::cout<<"Difference: "<<difference<<std::endl;
		}
	else
	//Calculat parameters directly
	mPhase();
	std::cout<<network_<<std::endl;	
}


/**calculateProbabilityEM
 * 
 * @param a reference to the query node
 * @param the query col
 * @param the query row
 *
 * @return the calculated probability, normalized to 1
 * 
 * Calculates the probability to observe a certain value
 */
float Controller::calculateProbabilityEM(Node& n, unsigned int col, unsigned int row){
	//get Parents
	auto ParentIDs = network_.getParents(n.getID());
	Matrix<float>& probMatrix = n.getProbabilityMatrix();
	//get Parent values
	std::vector<std::string> parentValues = split(probMatrix.getRowNames()[row],',');
	std::vector<std::string>& nodeValues= probMatrix.getColNames();
	//compute TotProbParentsRec
	float totProbParents=1.0;
	for (int key = 0; key<ParentIDs.size();key++){
		totProbParents*=computeTotalProbability(ParentIDs[key],parentValues[key]); 
	}
	//computeNormalizingProb
	float denominator=0.0;
	for (auto v : nodeValues){
		denominator+=probMatrix(probMatrix.findCol(v),row)*totProbParents;
		}
	//return result
	float nominator=probMatrix(col-1,row)*totProbParents;
	return nominator/denominator;
	}

/**ePhase
 * 
 * @return void
 * 
 * Performs the ePhase of the expectation maximization algorithm
 */
void Controller::ePhase(){
	for (auto& n:network_.getNodes()){
		Matrix<int>& obMatrix = n.getObservationMatrix();
		for (int row=0; row<obMatrix.getRowCount();row++){
			if (obMatrix.hasNACol()){
				for (int col=1;col<obMatrix.getColCount();col++){
					float value=obMatrix(col,row)+calculateProbabilityEM(n,col,row)*obMatrix(0,row);
					obMatrix.setData(value,col,row);
					}
				}
			else {
				for (int col=0;col<obMatrix.getColCount();col++){
					float value=obMatrix(col,row);
					obMatrix.setData(value,col,row);
					}
				}
			}
		}
	}

/**mPhase
 *
 * @return difference between the current and the previous parameters, normalised by the number of parameters
 * 
 * Carries out the mPhase of the EM algorithm. Also used to perform maxium likelihood estimation in the case of complete data
 */
float Controller::mPhase(){
	float difference=0.0;
	float probability=0.0;
	int counter=0;
	for (auto& n:network_.getNodes()){
		Matrix<int>& obMatrix = n.getObservationMatrix();
		for (int row=0; row<obMatrix.getRowCount(); row++){
				float rowsum=obMatrix.calculateRowSum(row);
				if (obMatrix.hasNACol())
				//Shift about 1
				for (int col=1;col<obMatrix.getColCount();col++){
					probability=n.getObservationMatrix()(col,row)/(rowsum-obMatrix(0,row));
					difference+=fabs(n.getProbability(col-1,row)-probability);
					n.setProbability(probability,col-1,row);
					counter++;
					}
				else
				//Data complet
				for (int col=0;col<obMatrix.getColCount();col++){
					probability=n.getObservationMatrix()(col,row)/rowsum;
					difference+=fabs(n.getProbability(col,row)-probability);
					n.setProbability(probability,col,row);
					counter++;
					}
			}
		n.loadBackup();
		}
	return difference/counter;
	}

/**initialise
 *
 * @param method encodes the method used for initilisation
 *
 * @return void 
 *
 * Performs initialising 0: Initialise with 1/#possible values, 1: Initialise with #Observation/#KnownObservations, 2: Initialise with (#Observations+#NA/#KnownObservations)/#RowSum
 */
void Controller::initialise(unsigned int method){
	switch(method){
		case 0: initialise1();
				break;
		case 1: initialise2();
				break;
		case 2:	initialise3();
				break;
		}	
	}


/*initialise1
 *
 * Initiliase with 1/#possible values
 */
void Controller::initialise1(){
	for (auto&n:network_.getNodes()){
		Matrix<int>& obMatrix=n.getObservationMatrix();
		Matrix<float>& probMatrix=n.getProbabilityMatrix();
		for (int row=0; row<probMatrix.getRowCount();row++){
			float rowsum=obMatrix.calculateRowSum(row);
			for(int col=0;col<probMatrix.getColCount();col++){
				n.setProbability(n.getObservationMatrix()(col+1,row)/(rowsum-n.getObservationMatrix()(0,row)),col,row);
				}  
			}
		}
	}


/*initialise2
 *
 *Initialise with #Observations/#KnownObservations
 */
void Controller::initialise2(){
    for (auto&n:network_.getNodes()){
        Matrix<int>& obMatrix=n.getObservationMatrix();
        Matrix<float>& probMatrix=n.getProbabilityMatrix();
        for (int row=0; row<probMatrix.getRowCount();row++){
            float rowsum=obMatrix.calculateRowSum(row);
            for(int col=0;col<probMatrix.getColCount();col++){
				if(obMatrix.hasNACol()){
					n.setProbability(n.getObservationMatrix()(col+1,row)/(rowsum-n.getObservationMatrix()(0,row)),col,row);
				}
				else {
					n.setProbability(n.getObservationMatrix()(col,row)/rowsum,col,row);
				}
			}
        }
    }
}

/*initalise3
 *
 * Initialise with (#Observations+#NA/#KnownObservations)/RowSum
 */
void Controller::initialise3(){
	for (auto&n:network_.getNodes()){
		Matrix<int>& obMatrix=n.getObservationMatrix();
		Matrix<float>& probMatrix=n.getProbabilityMatrix();
		for (int row=0; row<probMatrix.getRowCount();row++){
			float rowsum=obMatrix.calculateRowSum(row);
			for(int col=0;col<probMatrix.getColCount();col++){
    			if (obMatrix.hasNACol()){
					n.setProbability(((n.getObservationMatrix()(col+1,row)+(n.getObservationMatrix()(0,row)*(n.getObservationMatrix()(col+1,row)/(rowsum-n.getObservationMatrix()(0,row)))))/rowsum),col,row);
				}
				else {
					n.setProbability(n.getObservationMatrix()(col,row)/rowsum,col,row);
				}
        	}
        }
    }
}
