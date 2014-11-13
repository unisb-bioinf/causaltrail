#include "Network.h"
#include "boost/algorithm/string/split.hpp" 
#include "boost/algorithm/string/classification.hpp"

class Parser
{
	public:
	Parser(std::string userInput, Network& network);
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
	std::vector<int> nonInterventionNodeID_;
	std::unordered_map<int, unsigned int> nonInterventionValues_;
	std::vector<int> conditionNodeID_;
	std::unordered_map<int, unsigned int> conditionValues_;
	std::vector<int> doInterventionNodeID_;
	std::unordered_map<int, unsigned int> doInterventionValues_;
	std::vector<std::pair<int, int>> addEdgeNodeIDs_;
	std::vector<std::pair<int, int>> removeEdgeNodeIDs_;
	std::vector<int> argmaxNodeIDs_;
	Network& network_;
};
