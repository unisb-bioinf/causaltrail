#include "EM.h"


EM::EM(unsigned int method, Network& network, Matrix<int>& observations)
	:method_(method),network_(network),observations_(observations){
}


std::vector<std::string> EM::split(std::string& str, char delim){
    std::stringstream ss(str);
    std::string item;
    std::vector<std::string> result;
    while(std::getline(ss,item,delim)){
        result.push_back(item);
    }
    return result;
}


void EM::performEM(){
    float difference=1.0;
    int runs=0;
    //Check completness of the data
    if (observations_.contains(-1)){
        //Initialize and iterate E/M-Phase
        initalise();
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

float EM::computeTotalProbability(unsigned int nodeID, std::string value){
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


float EM::calculateProbabilityEM(Node& n, unsigned int col, unsigned int row){
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

void EM::ePhase(){
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

float EM::mPhase(){
	float difference=0.0;	
	float probability=0.0;
	int counter=0;	
	for (auto& n:network_.getNodes()){
		Matrix<int>& obMatrix = n.getObservationMatrix();
		for (int row=0; row<obMatrix.getRowCount(); row++){
			float rowsum=obMatrix.calculateRowSum(row);
			if (obMatrix.hasNACol())
				for (int col=1;col<obMatrix.getColCount();col++){
					probability=n.getObservationMatrix()(col,row)/(rowsum-obMatrix(0,row));
					difference+=fabs(n.getProbability(col-1,row)-probability);
					n.setProbability(probability,col-1,row);	
					counter++;
				}
			else {
				for (int col=0;col<obMatrix.getColCount();col++){
					probability=n.getObservationMatrix()(col,row)/rowsum;
					difference+=fabs(n.getProbability(col,row)-probability);
					n.setProbability(probability,col,row);
					counter++;
					}
				}
			}
        n.loadBackup();
		}
	return difference/counter;
}

void EM::initalise(){
	switch(method_){
		case 0: initalise1(); break;
		case 1: initalise2(); break;
		case 2: initalise3(); break;
	}
}

void EM::initalise1(){
	for (auto&n:network_.getNodes()){	
	Matrix<int>& obMatrix=n.getObservationMatrix();
		Matrix<float>& probMatrix=n.getProbabilityMatrix();
		for (int row=0; row<probMatrix.getRowCount();row++){	
			float rowsum=obMatrix.calculateRowSum(row);
			for(int col=0;col<probMatrix.getColCount();col++){
				n.setProbability(1/n.getUniqueValuesExcludingNA().size(),col,row);
        	}   
    	}   
	}   
}

void EM::initalise2(){
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

void EM::initalise3(){
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

