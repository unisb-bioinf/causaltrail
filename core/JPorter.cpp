#include "JPorter.h"
#include "iostream"
#include <boost/property_tree/json_parser.hpp>

JPorter::JPorter()
:intToMethod_(11,"NA")
{
	initialiseNameVector();
	initialiseMethodMap();
}

JPorter::~JPorter(){
}

void JPorter::initialiseMethodMap(){
	methodToInt_["Ceil"]=0;
	methodToInt_["Floor"]=1;
	methodToInt_["Round"]=2;
	methodToInt_["ArithmeticMean"]=3;
	methodToInt_["HarmonicMean"]=4;
	methodToInt_["Median"]=5;
	methodToInt_["Threshold"]=6;
	methodToInt_["BracketMedians"]=7;
	methodToInt_["PearsonTukey"]=8;
	methodToInt_["none"]=9;
	methodToInt_["Z-Score"]=10;
}

void JPorter::initialiseNameVector(){
	intToMethod_[0]="Ceil";
	intToMethod_[1]="Floor";
	intToMethod_[2]="Round";
	intToMethod_[3]="ArithmeticMean";
	intToMethod_[4]="HarmonicMean";
	intToMethod_[5]="Median";
	intToMethod_[6]="Threshold";
	intToMethod_[7]="BracketMedians";
	intToMethod_[8]="PearsonTukey";
	intToMethod_[9]="none";
	intToMethod_[10]="Z-Score";
}

void JPorter::importFile(const std::string& filename){
	boost::property_tree::read_json(filename,pt_);
}


void JPorter::exportToFile(const std::string& filename){
	boost::property_tree::write_json(filename,pt_);
}

void JPorter::printContent() const {
	for(auto& element:pt_){
		std::cout<<element.first<<std::endl;
		for (auto& property : element.second){
			std::cout<<property.first<<" "<<
			property.second.get_value<std::string>()<<std::endl;
		}
	}	
}

bool JPorter::containsNode(const std::string& nodeName) const{
	static std::string neg = "-1";
	if (pt_.get<std::string>(nodeName,neg) == neg)
		return false;
	else
		return true; 
}

unsigned int JPorter::convertMethodToIntegerCode(const std::string& method) const{
	auto it = methodToInt_.find(method);
	if (it == methodToInt_.end()){	
		throw std::invalid_argument("Method "+method+" not known.");
	}
	return it->second;
}

std::pair<unsigned int, float> JPorter::getDiscretisationDetails(const std::string& name) const{
	unsigned int method = convertMethodToIntegerCode(pt_.get<std::string>(name+".method"));
	if (method == 6) {
		float threshold = pt_.get<float>(name+".threshold");
		return std::make_pair(method,threshold);
	}
	else if (method == 7) {
		unsigned int buckets = pt_.get<unsigned int>(name+".buckets");
		return std::make_pair(method,buckets);
	}
	return std::make_pair(method,0);
}

void JPorter::addToTree(const std::string& name, unsigned int method){
	addToTree(name,intToMethod_[method]);
}

void JPorter::addToTree(const std::string& name, unsigned int method, float value){
	addToTree(name,intToMethod_[method],value);
}

void JPorter::addToTree(const std::string& name, const std::string& method){
	pt_.put(name+".method",method);
}

void JPorter::addToTree(const std::string& name, const std::string& method, float value){
	pt_.put(name+".method",method);
	if (method=="Threshold"){
		pt_.put(name+".threshold",std::to_string(value));
	}
	else if (method=="BracketMedians"){
		pt_.put(name+".buckets",std::to_string(value));
	}
}

