#include "Parser.h"
#include <string>
#include <complex>
 
/* struct query
 *
 * Query struct to store the parsed query information
 */
struct query{
	std::vector<int> nonInterventionNodeIDs;
	std::vector<std::string> nonInterventionNodeValues;
	std::vector<int> conditionNodeIDs;
	std::vector<std::string> conditionNodeValues;
	std::vector<int> doInterventionNodeIDs;
	std::vector<std::string> doInterventionNodeValues;
	std::vector<int> addEdgeNodeIDs;
	std::vector<std::string> removeEdgeNodeIDs;
	std::vector<int> argMaxNodeIDs;
};

template<typename Iterator>
struct query_parser : boost::spirit::qi::grammar<Iterator,boost::spirit::ascii::space_type>
{
	query_parser() : query_parser::base_type(query)	{
	
		using boost::spirit::ascii::string;
		using boost::spirit::qi::lit;
		using boost::spirit::qi::lexeme;
        using boost::spirit::ascii::char_;		
		using boost::spirit::qi::eoi;
	
		query = '?'
			>>(+nonIntervention >>(('!'	>>	+intervention) |	('|'	>>	+condition)	|	eoi))| +argMax;

		nonIntervention =
			(nodeName>>'='>>valueName);
		
		argMax = 
			(string("argmax")>>lit('(')>>+(nodeName)>>')');

		intervention =
			(string("do")	>> nodeName	>> '='	>> valueName)
			|('+' >>	lit('(')	>>	nodeName >>	','	>>	nodeName	>>	')')
			|('-' >>    lit('(') >>  nodeName >> ',' >>  nodeName   >> ')');

		condition =
			nodeName
			>>	'='
			>>	valueName;

		nodeName = lexeme[*(char_ -' ' -'=' - ')' - ',')];//"Grade";

		valueName = lexeme[*(char_-' ' -"do" -'+' -'-')];//"g";

			
	}
	boost::spirit::qi::rule<Iterator, boost::spirit::ascii::space_type> query;
	boost::spirit::qi::rule<Iterator, boost::spirit::ascii::space_type> nonIntervention;
	boost::spirit::qi::rule<Iterator, boost::spirit::ascii::space_type> argMax;
	boost::spirit::qi::rule<Iterator, boost::spirit::ascii::space_type> intervention;
	boost::spirit::qi::rule<Iterator, boost::spirit::ascii::space_type> condition;
	boost::spirit::qi::rule<Iterator, boost::spirit::ascii::space_type> nodeName;
	boost::spirit::qi::rule<Iterator, boost::spirit::ascii::space_type> valueName;

};


/* Parser
 *
 * constructs a Parser object
 *
 * @return Parser object
 */
Parser::Parser(const std::string userInput, Network& network)
	:userInput_(userInput), network_(network){
	query qs;
	using boost::spirit::ascii::space;
	query_parser<std::string::const_iterator> qp;
	std::cout<<phrase_parse(userInput.begin(),userInput.end(),qp,space)<<std::endl;
}
