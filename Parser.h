#ifndef PARSER_H
#define PARSER_H

#include "Network.h"
#include "QueryExecuter.h"
#include "boost/algorithm/string/split.hpp" 
#include "boost/algorithm/string/classification.hpp"

class Parser
{
	public:
	Parser(std::string userInput, NetworkController& networkController);
	QueryExecuter parseQuery();

	private:
	void parseNonIntervention(int& index);
	void parseInterventions(int& index);
	void parseCondition(int& index);
	void parseDoIntervention(int& index);
	void parseAddEdge(int& index);
	void parseRemoveEdge(int& index);
	void parseArgMax(int& index);
	bool getNode(const std::string& nodeName);
	bool getValue(const std::string& nodeName, const std::string& valueName);
	bool terminationSymbol(const int index) const;
	bool terminationSymbolArgMax(const int index) const;
	unsigned int getNodeID(const std::string& nodeName);
	unsigned int getValueID(const std::string& nodeName, const std::string& valueName);

	std::string userInput_;
	std::vector<std::string> query_;
	Network& network_;
	QueryExecuter qe_;
};

#endif
