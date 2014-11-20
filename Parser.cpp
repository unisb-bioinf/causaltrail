#include "Parser.h"
//#include <complex>

/* struct query
 *
 * Query struct to store the parsed query information
 */

QueryExecuter Parser::parseQuery()
{
	int index = 0;
	if(query_[index] != "?") {
		throw std::invalid_argument("In parseQuery, query should start with ?");
	}
	if(query_[1] == "argmax") {
		index = 2;
		parseArgMax(index);
	} else {
		index = 1;
		parseNonIntervention(index);
	}

	while(index < query_.size()) {
		if(query_[index] == "!") {
			index++;
			if (index < query_.size()){
				parseInterventions(index);
			}
			else {
				throw std::invalid_argument("In parseQuery, no Intervention specified");
			}
		} else if (query_[index] == "|") {
			index++;
			if (index < query_.size()){
				parseCondition(index); 
			}
			else {
				throw std::invalid_argument("In parseQuery, no Intervention specified");
			}
		}
		index++;
	}
	return qe_;
}

bool Parser::terminationSymbol(int index)
    const
{
	return index != query_.size() && query_[index] != "!" && query_[index] != "|";
}

bool Parser::terminationSymbolArgMax(int index) const
{
	if (index == query_.size()){
		throw std::invalid_argument("In terminationSymbolArgMax, index out of bound");
	}
	return query_[index] != ")";
}

void Parser::parseInterventions(int& index)
{
	bool ato=false;
	while(terminationSymbol(index)) {
		if(query_[index] == "do") {
			index++;
			parseDoIntervention(index); 
			ato=true;
		}
		if(query_[index] == "+") {
			index++;
			parseAddEdge(index);
			ato=true;
		}
		if(query_[index] == "-") {
			index++;
			parseRemoveEdge(index);
			ato=true;
		}
		index++;
		if (not ato){
			throw std::invalid_argument("In parseInterventions, no inverventions found");
		}
	}
}

void Parser::parseNonIntervention(int& index)
{
	while(terminationSymbol(index)) {
		if(not getNode(query_[index])) {
			throw std::invalid_argument("In parseNonIntervention, invalid node name "+query_[index]);
		}
		index++;
		if(query_[index] != "=") {
			throw std::invalid_argument("In parseNonIntervention, invalid sign expected = but found "+query_[index]);
		}
		index++;
		if(not getValue(query_[index - 2], query_[index])) {
			throw std::invalid_argument("in parsenNonIntervention, invalid value "+query_[index]);
		}
		else{
			qe_.setNonIntervention(getNodeID(query_[index-2]),getValueID(query_[index-2],query_[index]));
		}
		index++;
	}
}

void Parser::parseCondition(int& index)
{
	while(terminationSymbol(index)) {
		if(not getNode(query_[index])){
			throw std::invalid_argument("In parseCondition, invalid node name "+query_[index]);
		}
		index++;
		if(query_[index] != "=")
			throw std::invalid_argument("In parseCondition, invalid sign, expected = but found "+query_[index]);
		index++;
		if(not getValue(query_[index - 2], query_[index])){
			throw std::invalid_argument("In parseCondition, node "+query_[index-2]+" does not have a value "+query_[index]);
		}
		else{
			qe_.setCondition(getNodeID(query_[index-2]),getValueID(query_[index-2],query_[index]));
		}
		index++;
	}
}

void Parser::parseDoIntervention(int& index)
{
	if(not getNode(query_[index])){
		throw std::invalid_argument("In parseDoIntervention, invalid node name "+query_[index]);
	}
	index++;
	if(query_[index] != "=")
		throw std::invalid_argument("In parseDoIntervention, invalid sign expected = but found "+query_[index]);
	index++;
	if(not getValue(query_[index - 2], query_[index])){
		throw std::invalid_argument("In parseDoIntervention, node "+query_[index-2]+" does not have a value "+query_[index]);
	}
	else{
		qe_.setDoIntervention(getNodeID(query_[index-2]),getValueID(query_[index-2],query_[index]));
	}
}

void Parser::parseAddEdge(int& index)
{
	if ((index+2) < query_.size()){
		if(not getNode(query_[index])){
			throw std::invalid_argument("In parseAddEdge, invalid node name "+query_[index]);
		}
		index++;
		if(not getNode(query_[index])){
			throw std::invalid_argument("In parseAddEdge, invalid node name "+query_[index]);
		}
		qe_.setAddEdge(getNodeID(query_[index-1]),getNodeID(query_[index]));	
	}
	else {
		throw std::invalid_argument("Invalid NodeIndex");
	}
}

void Parser::parseRemoveEdge(int& index)
{
	if ((index+2) < query_.size()){
		if(not getNode(query_[index])){
			throw std::invalid_argument("In parseRemoveEdge, invalid node name "+query_[index]);
		}
		index++;
		if(not getNode(query_[index])){
			throw std::invalid_argument("In parseRemoveEdge, invalid node name "+query_[index]);
		}
		qe_.setRemoveEdge(getNodeID(query_[index-1]),getNodeID(query_[index]));
	}
	else{
		throw std::invalid_argument("Invalid NodeIndex");
	}

}

void Parser::parseArgMax(int& index)
{
	if(query_[index] != ("("))
		throw std::invalid_argument("In parseArgMax, invalid sign expected ( but found  "+query_[index]);
	index++;
	while(terminationSymbolArgMax(index)) {
		if(not getNode(query_[index]))
			throw std::invalid_argument("Invalid node name "+query_[index]);
		else {
		qe_.setArgMax(getNodeID(query_[index]));
		}
		index++;
	}
}

bool Parser::getNode(const std::string& nodeName)
{
	return network_.hasNode(nodeName);
}

bool Parser::getValue(const std::string& nodeName, const std::string& valueName)
{
	return network_.hasValue(nodeName, valueName);
}

unsigned int Parser::getNodeID(const std::string& nodeName){
	return network_.getNode(nodeName).getID();
}

unsigned int Parser::getValueID(const std::string& nodeName, const std::string& valueName){
	return network_.getNode(nodeName).getIndex(valueName);
}

/* Parser
 *
 * constructs a Parser object
 *
 * @return Parser object
 */
Parser::Parser(std::string userInput, NetworkController& networkController)
    : userInput_(userInput), network_(networkController.getNetwork()), qe_(QueryExecuter(networkController))
{
	boost::split(query_, userInput_, boost::is_any_of(" "));
}

