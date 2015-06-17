#include "DiscretisationFactory.h"
#include "Discretisations.h"
#include "DiscretiseFloor.h"
#include "DiscretiseArithmeticMean.h"
#include "DiscretiseMedian.h"
#include "DiscretiseRound.h"
#include "DiscretiseBracketMedians.h"
#include "DiscretiseHarmonicMean.h"  
#include "DiscretisePT.h"      
#include "DiscretiseThreshold.h"
#include "DiscretiseCeil.h"            
#include "DiscretiseMapping.h"       
#include "DiscretiseZScore.h"

DiscretisationFactory::DiscretisationFactory(SerializeDeserializeJson& jsonTree,
					const Matrix<std::string>& originalObservations, 
					Matrix<int>& discretisedObservations,
				        std::unordered_map<std::string,int>& observationsMap,
        				std::map<std::pair<int,int>, std::string>& observationsMapR)
	:jsonTree_(jsonTree),
	originalObservations_(originalObservations),
	discretisedObservations_(discretisedObservations),
        observationsMap_(observationsMap),
        observationsMapR_(observationsMapR)
{
	methodIndex_["Ceil"]=0;
	methodIndex_["Floor"]=1;
	methodIndex_["Round"]=2;
	methodIndex_["ArithmeticMean"]=3;
	methodIndex_["HarmonicMean"]=4;
	methodIndex_["Median"]=5;
	methodIndex_["Threshold"]=6;
	methodIndex_["BracketMedians"]=7;
	methodIndex_["PearsonTukey"]=8;
	methodIndex_["None"]=9;
	methodIndex_["Z-Score"]=10;
}


unsigned int DiscretisationFactory::getMethodIndex(const std::string& methodS){
	auto it = methodIndex_.find(methodS);
	if (it != methodIndex_.end()){
		return it->second;
	}
	else {
		throw std::invalid_argument("Method: "+methodS+" not known.");
	}
}

std::unique_ptr<Discretisations> DiscretisationFactory::create(const std::string& nodeName, unsigned int row){
	
	if(jsonTree_.containsNode(nodeName)){
		const std::string& methodS = jsonTree_.getMethod(nodeName);
		unsigned int method = getMethodIndex(methodS);
		switch(method) {
			case 0:
			{	
				std::unique_ptr<Discretisations> ptr (new DiscretiseCeil(row,
					originalObservations_,
					discretisedObservations_,
					observationsMap_,
					observationsMapR_));
				return ptr;
			}
			case 1:
			{	
				std::unique_ptr<Discretisations> ptr(new DiscretiseFloor(row,
					originalObservations_, 
					discretisedObservations_,
					observationsMap_,
					observationsMapR_));
				return ptr;
			}
			case 2:
			{		
				std::unique_ptr<Discretisations> ptr(new DiscretiseRound(row,
					originalObservations_, 
					discretisedObservations_,
					observationsMap_,
					observationsMapR_));
				return ptr;
			}
			case 3:
			{
				std::unique_ptr<Discretisations> ptr (new DiscretiseArithmeticMean(row,
					originalObservations_, 
					discretisedObservations_,
					observationsMap_,
					observationsMapR_));
				return ptr;
			}
			case 4:
			{
				std::unique_ptr<Discretisations> ptr (new DiscretiseHarmonicMean(row,
					originalObservations_, 
					discretisedObservations_,
					observationsMap_,
					observationsMapR_));
				return ptr;
			}
			case 5:
			{
				std::unique_ptr<Discretisations> ptr (new DiscretiseMedian(row,
					originalObservations_, 
					discretisedObservations_,
					observationsMap_,
					observationsMapR_));
				return ptr;
			}
			case 6:
			{	
				float threshold = jsonTree_. getParameter<float>(nodeName,"threshold");
				std::unique_ptr<Discretisations> ptr (new DiscretiseThreshold(row,
					originalObservations_, 
					discretisedObservations_,
					observationsMap_,
					observationsMapR_,
					threshold));
				return ptr;	
			}
			case 7:
			{
				int buckets = jsonTree_.getParameter<int>(nodeName,"buckets");
				std::unique_ptr<Discretisations> ptr (new DiscretiseBracketMedians(row,
					originalObservations_, 
					discretisedObservations_,
					observationsMap_,
					observationsMapR_,
					buckets));
				return ptr;
			}
			case 8:
			{		
				std::unique_ptr<Discretisations> ptr (new DiscretisePT(row,
					originalObservations_, 
					discretisedObservations_,
					observationsMap_,
					observationsMapR_));
				return ptr;
			}
			case 9:
			{		
				std::unique_ptr<Discretisations> ptr (new DiscretiseMapping(row,
					originalObservations_, 
					discretisedObservations_,
					observationsMap_,
					observationsMapR_));
				return ptr;
			}
			case 10:
			{	
				std::unique_ptr<Discretisations> ptr (new DiscretiseZScore(row,
					originalObservations_, 
					discretisedObservations_,
					observationsMap_,
					observationsMapR_));
				return ptr;
			}
		}
	
		throw std::runtime_error("Method Index not found!");	
	}
	else {
		throw std::invalid_argument("Node "+nodeName+" not found");
	}
}

