#ifndef PARSER_H
#define PARSER_H

#include "Network.h"
#include "QueryExecuter.h"

class Parser
{
	public:

	/**
	 * Parser for the do-calculus query language
	 *
	 * @param userInput the query entered by the user
	 * @param networkController a reference to a NetworkController object
	 */ 
	Parser(std::string userInput, NetworkController& networkController);

	/**
	 * Parses the given query. Thereby the method generates a QueryExecuter
	 * object that can execute the parsed query
	 *
	 * @return a QueryExecuter object
	 */
	QueryExecuter parseQuery();

	private:

	/**
	 * Parses a NonIntervention
	 * NonIntervention ::= +(NodeName = Value)
	 *
	 * @param index current position in the query token list
	 */
	void parseNonIntervention(unsigned int& index);

	/**
	 * Parses an Intervention
	 * Intervention ::= +DoIntervention | +AddEdge | +RemoveEdge
	 *
	 * @param index current position in the query token list
	 */
	void parseInterventions(unsigned int& index);

	/**
	 * Parses a Condition
	 * Condition ::= +(NodeName = Value)
	 *
	 * @param index current position in the query token list
	 */
	void parseCondition(unsigned int& index);

	/**
	 * Parses a Do-Intervention
	 * DoIntervention ::= do NodeName = Value
	 *
	 * @param index current position in the query token list
	 */
	void parseDoIntervention(unsigned int& index);

	/**
	 * Parses an edge addition
	 * AddEdge ::= + ( NodeName , NodeName )
	 *
	 * @param index current position in the query token list
	 */
	void parseAddEdge(unsigned int& index);

	/**
	 * Parses an edge removal
	 * RemoveEdge ::= - ( NodeName , NodeName )
	 *
	 * @param index current position in the query token list
	 */
	void parseRemoveEdge(unsigned int& index);

	/**
	 * parses a MAP query
	 * ArgMax ::= argmax ( +NodeName ) *Intervention *Condition
	 *
	 * @param index current position in the query token list
	 */
	void parseArgMax(unsigned int& index);

	/**getNode
	 *
	 * @param nodeName name of the node to be retrieved
	 *
	 * @return true, if the node with the given excists, false otherwise
	 */
	bool getNode(const std::string& nodeName);

	/**getValue
	 *
	 * @param nodeName name of the node that should contain a certain value
	 * @param valueName name of the value to be retrieved
	 *
	 * @return true, if the given node contains the given value, false otherwise
	 */
	bool getValue(const std::string& nodeName, const std::string& valueName);

	/**terminationSymbol
	 *
	 * @param index current position in the query token list
	 *
	 * @return true if the symbol at the given position is a terminal symbol, false otherwise
	 * TerminationSymbol = ! | ""
	 */
	bool terminationSymbol(const unsigned int index) const;

	/**parseNonIntervention
	 *
	 * @param index current position in the query token list
	 *
	 * @return true if the symbol at the given position is a terminal symbol for MAP queries, false otherwise
	 *
 	 * TerminationSymbol = ) 
	 */
	bool terminationSymbolArgMax(const unsigned int index) const;

	/**parseNonIntervention
	 *
	 * @param nodeName name of the node for whom we want to know the identifier
	 *
	 * @return the identifier of the node with the given nodeName
	 */
	unsigned int getNodeID(const std::string& nodeName);
	
	/**parseNonIntervention
	 *
	 * @param nodeName name of the node in focus
	 * @param valueName original representation of a value in the query
	 *
	 * @return the internal integer value for the given node and value in the original representation
	 */
	unsigned int getValueID(const std::string& nodeName, const std::string& valueName);

	//original query entered by the user
	std::string userInput_;

	//tokenized query
	std::vector<std::string> query_;

	//A reference to the network
	Network& network_;

	//The QueryExecuter object that is returned once the parsing finished successfully
	QueryExecuter qe_;
};

#endif
