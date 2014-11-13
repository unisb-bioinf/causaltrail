#include "Parser.h"
//#include <complex>

/* struct query
 *
 * Query struct to store the parsed query information
 */

bool Parser::parseQuery(const std::vector<std::string>& query)
{
	int index = 0;
	if(query[index] != "?") {
		return false;
	}
	if(query[1] == "argmax") {
		index = 2;
		parseArgMax(query, index);
	} else {
		index = 1;
		parseNonIntervention(query, index);
	}

	while(index < query.size()) {
		if(query[index] == "!") {
			index++;
			if(not parseInterventions(query, index)) {
				return false;
			}
		} else {
			index++;
			if(not parseCondition(query, index)) {
				return false;
			}
		}
	}
	return true;
}

bool Parser::terminationSymbol(const std::vector<std::string>& sym, int index)
    const
{
	return index != sym.size() && sym[index] != "!" && sym[index] != "|";
}

bool Parser::terminationSymbolArgMax(const std::vector<std::string>& sym,
                                     int index) const
{
	return index != sym.size() && sym[index] != ")";
}

bool Parser::parseInterventions(const std::vector<std::string>& query,
                                int& index)
{
	while(terminationSymbol(query, index)) {
		if(query[index] == "do") {
			index++;
			if(not parseDoIntervention(query, index)) {
				return false;
			}
		}
		if(query[index] == "+") {
			index++;
			if(not parseAddEdge(query, index)) {
				return false;
			}
		}
		if(query[index] == "-") {
			index++;
			if(not parseRemoveEdge(query, index)) {
				return false;
			}
		}
		index++;
	}
	return true;
}

bool Parser::parseNonIntervention(const std::vector<std::string>& query,
                                  int& index)
{
	while(terminationSymbol(query, index)) {
		if(not getNode(query[index])) {
			return false;
		}
		else{
			nonInterventionNodeID_.push_back(getNodeID(query[index]));
		}
		index++;
		if(query[index] != "=") {
			return false;
		}
		index++;
		if(not getValue(query[index - 2], query[index])) {
			return false;
		}
		else{
			nonInterventionValues_[getNodeID(query[index-2])]=getValueID(query[index-2],query[index]);
		}
		index++;
	}
	return true;
}

bool Parser::parseCondition(const std::vector<std::string>& query, int& index)
{
	while(terminationSymbol(query, index)) {
		if(not getNode(query[index])){
			throw std::invalid_argument("In parseCondition, invalid node name "+query[index]);
		}
		else{
			conditionNodeID_.push_back(getNodeID(query[index]));
		}
		index++;
		if(query[index] != "=")
			throw std::invalid_argument("In parseCondition, invalid sign, expected = but found "+query[index]);
		index++;
		if(not getValue(query[index - 2], query[index])){
			throw std::invalid_argument("In parseCondition, node "+query[index-2]+" does not have a value "+query[index]);
		}
		else{
			conditionValues_[getNodeID(query[index-2])]=getValueID(query[index-2],query[index]);
		}
		index++;
	}
	return true;
}

bool Parser::parseDoIntervention(const std::vector<std::string>& query,
                                 int& index)
{
	if(not getNode(query[index])){
		throw std::invalid_argument("In parseDoIntervention, invalid node name "+query[index]);
	}
	else{
		doInterventionNodeID_.push_back(getNodeID(query[index]));
	}
	index++;
	if(query[index] != "=")
		throw std::invalid_argument("In parseDoIntervention, invalid sign expected = but found "+query[index]);
	index++;
	if(not getValue(query[index - 2], query[index])){
		throw std::invalid_argument("In parseDoIntervention, node "+query[index-2]+" does not have a value "+query[index]);
	}
	else{
		doInterventionValues_[getNodeID(query[index-2])]=getValueID(query[index-2],query[index]);
	}
	return true;
}

bool Parser::parseAddEdge(const std::vector<std::string>& query, int& index)
{
	if(not getNode(query[index])){
		throw std::invalid_argument("In parseAddEdge, invalid node name "+query[index]);
	}
	index++;
	if(not getNode(query[index])){
		throw std::invalid_argument("In parseAddEdge, invalid node name "+query[index]);
	}
	addEdgeNodeIDs_.push_back(std::make_pair(getNodeID(query[index-2]),getNodeID(query[index])));	
	return true;
}

bool Parser::parseRemoveEdge(const std::vector<std::string>& query, int& index)
{
	if(not getNode(query[index])){
		throw std::invalid_argument("In parseRemoveEdge, invalid node name "+query[index]);
	}
	index++;
	if(not getNode(query[index])){
		throw std::invalid_argument("In parseRemoveEdge, invalid node name "+query[index]);
	}
	removeEdgeNodeIDs_.push_back(std::make_pair(getNodeID(query[index-2]),getNodeID(query[index])));
	return true;
}

bool Parser::parseArgMax(const std::vector<std::string>& query, int& index)
{
	if(query[index] != ("("))
		throw std::invalid_argument("In parseArgMax, invalid sign expected ( but found  "+query[index]);
	index++;
	while(terminationSymbolArgMax(query, index)) {
		if(not getNode(query[index]))
			throw std::invalid_argument("Invalid node name "+query[index]);
		else {
		argmaxNodeIDs_.push_back(getNodeID(query[index]));
		}
		index++;
	}
	return true;
}

bool Parser::getNode(const std::string& nodeName)
{
	return network_.hasNode(nodeName);
}

bool Parser::getValue(const std::string& nodeName, const std::string& valueName)
{
	return network_.hasValue(nodeName, valueName);
}

int Parser::getNodeID(const std::string& nodeName){
	return network_.getNode(nodeName).getID();
}

int Parser::getValueID(const std::string& nodeName, const std::string& valueName){
	return network_.getNode(nodeName).getIndex(valueName);
}

/* Parser
 *
 * constructs a Parser object
 *
 * @return Parser object
 */
Parser::Parser(const std::string userInput, Network& network)
    : userInput_(userInput), network_(network)
{
	boost::split(query_, userInput_, boost::is_any_of(" "));
	try{
	std::cout << parseQuery(query_) << std::endl;
	}
	catch (const std::invalid_argument& ia){
		std::cerr<<ia.what()<<std::endl;
		}
}

