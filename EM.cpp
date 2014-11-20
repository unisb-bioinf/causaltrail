#include "EM.h"

/*
 *
 *
 *
 *
 *
 *
 */
EM::EM(Network& network, Matrix<int>& observations, float difference, unsigned int runs)
	:network_(network),observations_(observations),probHandler_(ProbabilityHandler(network)), differenceThreshold_(difference), maxRuns_(runs){
	performEM();
}

/*
 *
 *
 *
 *
 *
 *
 */
void EM::performEM(){
	unsigned int runs=0;
	float difference=1.0f;
	float maxprob=0.0;
	unsigned int maxmethod=0;
    //Check completness of the data
    if (observations_.contains(-1)){
		for (int method=0; method<2; method++){
	        //Initialize and iterate E/M-Phase
			method_=method;
	        initalise();
	        while ((difference > differenceThreshold_) and (runs < maxRuns_)){
	            ePhase();
	            difference=mPhase();
	            runs++;
			}
	        std::cout<<"Runs: "<<runs<<std::endl;
	        std::cout<<"Difference: "<<difference<<std::endl;
			float prob = calculateLikelihoodOfTheData();
			std::cout<<"Probability of the data: "<<prob<<std::endl;
			if (prob > maxprob){
				maxmethod=method;
			}
		}
		method_=maxmethod;
		initalise();
		difference=1.0f;
		runs=0;
		while ((difference > differenceThreshold_) and (runs < maxRuns_)){
			ePhase();
			difference=mPhase();
			runs++;
		}
	}
    else{
    //Calculat parameters directly
	    mPhase();
	}
    std::cout<<network_<<std::endl;
}

/*
 *
 *
 *
 *
 *
 *
 */
float EM::calculateProbabilityEM(Node& n, unsigned int col, unsigned int row){
    //get Parents
    auto ParentIDs = n.getParents();
	Matrix<float>& probMatrix = n.getProbabilityMatrix();

    //compute TotProbParentsRec
    float totProbParents=1.0;
    for (int key = 0; key<ParentIDs.size();key++){
        totProbParents*=probHandler_.computeTotalProbability(ParentIDs[key], network_.reverseFactor(n,ParentIDs[key],row));
    }

    //computeNormalizingProb
    float denominator=0.0;
    for (unsigned int col = 0; col<probMatrix.getColCount(); col++){
        denominator+=probMatrix(col,row)*totProbParents;
	}

    //return result
    float nominator=probMatrix(col-1,row)*totProbParents;
    return nominator/denominator;
}

/*
 *
 *
 *
 *
 *
 *
 */
void EM::calculateExpectedValue(unsigned int row, Node& n, Matrix<int>& obMatrix){
	if (obMatrix.hasNACol()){
		for (int col=1;col<obMatrix.getColCount();col++){
			float value=obMatrix(col,row)+calculateProbabilityEM(n,col,row)*obMatrix(0,row);
			obMatrix.setData(value,col,row);
		}
	}
}

/*
 *
 *
 *
 *
 *
 *
 */
void EM::ePhase(){
	for (auto& n:network_.getNodes()){
		Matrix<int>& obMatrix = n.getObservationMatrix();
		for (unsigned int row=0; row<obMatrix.getRowCount();row++){
			calculateExpectedValue(row,n,obMatrix);
		}
	}
}

/*
 *
 *
 *
 *
 *
 *
 */
void EM::calculateMaximumLikelihood(unsigned int row, unsigned int& counter, float& difference, Node& n, const Matrix<int>& obMatrix){
	float rowsum=obMatrix.calculateRowSum(row); 
	if (obMatrix.hasNACol())
		for (unsigned int col=1;col<obMatrix.getColCount();col++){
			float probability=n.getObservationMatrix()(col,row)/(rowsum-obMatrix(0,row));	
			difference+=fabs(n.getProbability(col-1,row)-probability);
			n.setProbability(probability,col-1,row);	
			counter++;
		}
	else {
		for (unsigned int col=0;col<obMatrix.getColCount();col++){
			float probability=n.getObservationMatrix()(col,row)/rowsum;
			difference+=fabs(n.getProbability(col,row)-probability);
			n.setProbability(probability,col,row);
			counter++;
		}
	}
}

/*
 *
 *
 *
 *
 *
 *
 */
float EM::mPhase(){
	float difference=0.0f;	
	float probability=0.0f;
	unsigned int counter=0;	
	for (auto& n:network_.getNodes()){
		const Matrix<int>& obMatrix = n.getObservationMatrix();
		for (unsigned int row=0; row<obMatrix.getRowCount(); row++)
			calculateMaximumLikelihood(row, counter, difference, n, obMatrix);
        n.loadBackup();
		}
	return difference/counter;
}

/*
 *
 *
 *
 *
 *
 *
 */
void EM::initalise(){
	switch(method_){
		case 0: initalise1(); break;
		case 1: initalise2(); break;
	}
}

/*
 *
 *
 *
 *
 *
 *
 */
void EM::initalise1(){
	for (auto&n:network_.getNodes()){	
		const Matrix<int>& obMatrix=n.getObservationMatrix();
		Matrix<float>& probMatrix=n.getProbabilityMatrix();
		for (int row=0; row<probMatrix.getRowCount();row++){	
			float rowsum=obMatrix.calculateRowSum(row);
			for(int col=0;col<probMatrix.getColCount();col++){
				n.setProbability(1.0f/n.getUniqueValuesExcludingNA().size(),col,row);
        	}   
    	}   
	}   
}

/*
 *
 *
 *
 *
 *
 *
 */
void EM::initalise2(){
    for (auto&n:network_.getNodes()){
        const Matrix<int>& obMatrix=n.getObservationMatrix();
        Matrix<float>& probMatrix=n.getProbabilityMatrix();
        for (int row=0; row<probMatrix.getRowCount();row++){
            float rowsum=obMatrix.calculateRowSum(row);
            for(int col=0;col<probMatrix.getColCount();col++){
                if(obMatrix.hasNACol()){
                    n.setProbability(n.getObservationMatrix()(col+1,row)/
									(rowsum-n.getObservationMatrix()(0,row))
									,col,row);
                }   
                else {
                    n.setProbability(n.getObservationMatrix()(col,row)/rowsum,col,row);
                }   
            }   
        }   
    }  
}

/**loadSample
 *
 * @nodes
 * 
 * @return values
 *
 **/
std::unordered_map<unsigned int, int> EM::loadSample(std::vector<unsigned int> nodes, unsigned int sample){
	std::unordered_map<unsigned int, int> sampleValues;
	for (auto& id : nodes){
		Node& n = network_.getNode(id);
		int obsRow = n.getObservationRow();
		sampleValues[id]=observations_(sample,obsRow);
	}
	return sampleValues;
}


/*calculateLikelihoodOfTheData
 *
 *
 */
float EM::calculateLikelihoodOfTheData(){
	float prob = 0.0f;
	std::vector<unsigned int> nodes = network_.getNodeIDs();
	for (unsigned int sample=0; sample<observations_.getColCount();sample++){
		if (not observations_.containsElement(0,sample,-1))
		{
			std::unordered_map<unsigned int, int> values=loadSample(nodes, sample);
			prob += probHandler_.computeJointProbability(nodes,values);
		}
	}
	return log(prob);
}
