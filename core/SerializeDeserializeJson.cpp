#include "SerializeDeserializeJson.h"
#include "iostream"
#include <boost/property_tree/json_parser.hpp>

SerializeDeserializeJson::SerializeDeserializeJson()
{
}


SerializeDeserializeJson::SerializeDeserializeJson(const std::string& filename)
{
	importFile(filename);
}


SerializeDeserializeJson::~SerializeDeserializeJson(){
}

void SerializeDeserializeJson::importFile(const std::string& filename){
	boost::property_tree::read_json(filename,pt_);
}


void SerializeDeserializeJson::exportToFile(const std::string& filename){
	boost::property_tree::write_json(filename,pt_);
}

bool SerializeDeserializeJson::containsNode(const std::string& nodeName) const{
	static std::string neg = "-1";
	if (pt_.get<std::string>(nodeName,neg) == neg)
		return false;
	else
		return true; 
}

const std::string SerializeDeserializeJson::getMethod(const std::string& nodeName) const{
	return pt_.get<std::string>(nodeName+".method");
}


void SerializeDeserializeJson::addToTree(const std::string& nodeName, const std::string& method){
	pt_.put(nodeName+".method",method);
}

void SerializeDeserializeJson::addToTree(const std::string& nodeName, const std::string& method, const std::string& parameterID, const std::string& value){
	pt_.put(nodeName+".method",method);
	pt_.put(nodeName+"."+parameterID,value);
}

