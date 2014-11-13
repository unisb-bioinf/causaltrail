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
	bool parseQuery(const std::vector<std::string>& query);

	private:
	bool parseNonIntervention(const std::vector<std::string>& query,
	                         int& index);
	bool parseInterventions(const std::vector<std::string>& query,
	                        int& index);
	bool parseCondition(const std::vector<std::string>& query,
	                   int& index);
	bool parseDoIntervention(const std::vector<std::string>& query,
	                        int& index);
	bool parseAddEdge(const std::vector<std::string>& query,
	                  int& index);
	bool parseRemoveEdge(const std::vector<std::string>& query,
	                      int& index);
	bool parseArgMax(const std::vector<std::string>& query,
	                  int& index);
	bool getNode(const std::string& nodeName);
	bool getValue(const std::string& nodeName, const std::string& valueName);
	bool terminationSymbol(const std::vector<std::string>& sym, const
	                       int index) const;
	bool terminationSymbolArgMax(const std::vector<std::string>& sym, const
	                             int index) const;
	int getNodeID(const std::string& nodeName);
	int getValueID(const std::string& nodeName, const std::string& valueName);

	std::string userInput_;
	std::vector<std::string> query_;
	Network& network_;
	QueryExecuter qe_;
};

#endif
