#include "DotReader.h"

#include <boost/phoenix/phoenix.hpp>

#define BOOST_SPIRIT_USE_PHOENIX_V3
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>

namespace qi    = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phx   = boost::phoenix;

BOOST_FUSION_ADAPT_STRUCT(
	Dot::NodeId,
	(std::string, name)
	(std::string, port)
	(std::string, compass_port)
)

BOOST_FUSION_ADAPT_STRUCT(
	Dot::Node,
	(Dot::NodeId, name)
	(Dot::Attributes, attributes)
)

BOOST_FUSION_ADAPT_STRUCT(
	Dot::Edge,
	(Dot::NodeId, source)
	(Dot::NodeId, target)
	(Dot::Attributes, attributes)
)

namespace Dot
{

NodeId::NodeId(const std::string& n, const std::string& p,
                          const std::string& cp)
	: name(n), port(p), compass_port(cp)
{ }

void NodeId::normalize() {
	if(compass_port != "") {
		return;
	}

	if(port == "n" || port == "ne" || port == "nw" ||
	   port == "s" || port == "se" || port == "sw" ||
	   port == "e" || port == "w"  || port == "c"  || port == "_")
	{
		std::swap(port, compass_port);
	}
}

/**
 * This is an implementation of the DOT language grammar as defined on
 * http://www.graphviz.org/doc/info/lang.html
 */
template <typename Iterator>
struct DotGrammar : qi::grammar<Iterator, Graph(), ascii::space_type>
{
	static std::vector<Edge> buildEdges(
		const std::vector<NodeId>& nodes,
		const boost::optional<Attributes>& attr)
	{
		std::vector<Edge> result;
		result.reserve(nodes.size() - 1);

		auto it = nodes.begin();
		auto jt = it + 1;

		Attributes def;
		for(; jt != nodes.end(); ++it, ++jt) {
			result.emplace_back(*it, *jt, attr.get_value_or(def));
		}

		return result;
	}

	DotGrammar() : DotGrammar::base_type(graph)
	{
		// Grammar rules
		graph
			=  -qi::lit("strict")
			>> directed          [phx::bind(&Graph::isDirected, qi::_val) = qi::_1]
			>> -id               [phx::bind(&Graph::name, qi::_val) = qi::_1]
			>> '{'
			>> stmt_list(qi::_val)
			>> '}';

		directed
			= qi::lit("graph")  [qi::_val = false]
			| qi::lit("digraph")[qi::_val = true]
			;

		stmt_list = (stmt(qi::_r1) % ';') >> -qi::lit(';');

		stmt
			= attr_stmt(qi::_r1)
			| edge_stmt[
				phx::insert(
					phx::bind(&Graph::edges, qi::_r1),
					phx::begin(phx::bind(&Graph::edges, qi::_r1)),
					phx::begin(qi::_1),
					phx::end(qi::_1)
				)]
			| node_stmt[phx::push_back(phx::bind(&Graph::nodes, qi::_r1), qi::_1)]
			| assign //TODO: Do something with this
			;

		attr_stmt
			= (qi::lit("graph") >> attr_list[phx::bind(&Graph::graph_attributes, qi::_r1) = qi::_1])
			| (qi::lit("node")  >> attr_list[phx::bind(&Graph::node_attributes,  qi::_r1) = qi::_1])
			| (qi::lit("edge")  >> attr_list[phx::bind(&Graph::edge_attributes,  qi::_r1) = qi::_1])
			;

		attr_list = '[' >> -(assign % qi::char_(";,")) >> ']';

		edge_stmt = (edge_list >> -attr_list)[
			qi::_val = phx::bind(&DotGrammar::buildEdges, qi::_1, qi::_2)
		];

		edge_list = node_id >> +(edge_op >> node_id);
		edge_op = qi::lit("--") | qi::lit("->");

		node_stmt = node_id >> -attr_list;
		node_id = id >> -(':' >> id >> -(':' >> compass_pt));

		compass_pt
			= qi::string("ne") | qi::string("nw") | qi::string("se")
			| qi::string("sw") | qi::string("n")  | qi::string("s")
			| qi::string("w")  | qi::string("e")  | qi::string("c") | qi::string("_")
			;

		// Auxillary rules
		id
			= qi::char_("a-zA-Z_\200-\377") >> *qi::char_("a-zA-Z_\200-\3770-9")
			| qi::lexeme['"' >> *(qi::char_ - '"') >> '"']
			| -qi::char_('-') >>
				(
					(qi::char_('.') >> +qi::char_("0-9")) |
					(+qi::char_("0-9") >> -(qi::char_('.') >> *qi::char_("0-9")))
				)
			;

		assign = id >> qi::lit('=') >> id;
	}

	// Auxillary rules not mentioned in the original grammar
	qi::rule<Iterator, bool(), ascii::space_type> directed;
	qi::rule<Iterator, std::pair<std::string, std::string>(), ascii::space_type> assign;
	qi::rule<Iterator, std::vector<NodeId>(), ascii::space_type> edge_list;

	// Rules mentioned in the original grammar
	qi::rule<Iterator, std::string(), ascii::space_type> id, compass_pt;
	qi::rule<Iterator, NodeId(), ascii::space_type> node_id;
	qi::rule<Iterator, Attributes(), ascii::space_type> attr_list;
	qi::rule<Iterator, Node(), ascii::space_type> node_stmt;
	qi::rule<Iterator, std::vector<Edge>(), ascii::space_type> edge_stmt;
	qi::rule<Iterator, Graph(), ascii::space_type> graph;

	qi::rule<Iterator, void(Graph&), ascii::space_type> stmt, stmt_list, attr_stmt;
	qi::rule<Iterator, ascii::space_type> edge_op;
};

bool Reader::parse(const char* begin, const char* end)
{
	using iterator_type = const char*;
	using dot_parser = DotGrammar<iterator_type>;

	static const dot_parser parser;

	// Clear the current network
	network_ = Graph();
	bool result = qi::phrase_parse(begin, end, parser, ascii::space, network_);

	if(!result) {
		std::string remainder(begin, end);
		std::cout << "Parsing failed.\nStopped at: \"" << remainder << "\"\n";

		// Clear everything we parsed
		network_ = Graph();
	}

	postProcess_();

	return result;
}

bool Graph::hasNode(const NodeId& id) const {
	for(const auto& n : nodes) {
		if(n.name.name == id.name) {
			return true;
		}
	}

	return false;
}

bool Reader::postProcess_() {
	for(auto& n : network_.nodes) {
		n.name.normalize();
	}

	for(auto& e : network_.edges) {
		e.source.normalize();
		if(!network_.hasNode(e.source)) {
			network_.nodes.emplace_back(e.source);
		}

		e.target.normalize();
		if(!network_.hasNode(e.target)) {
			network_.nodes.emplace_back(e.target);
		}
	}

	return true;
}

const Graph& Reader::getGraph() const
{
	return network_;
}

} // namespace Dot
