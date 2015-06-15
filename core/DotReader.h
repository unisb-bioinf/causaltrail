#ifndef DOT_READER_H
#define DOT_READER_H

#include <map>
#include <string>
#include <vector>

namespace Dot
{

using Attributes = std::map<std::string, std::string>;

struct NodeId {
	NodeId(const std::string& n = "", const std::string& p = "",
	       const std::string& cp = "");

	void normalize();

	bool operator==(const NodeId& o) const
	{
		return name == o.name
		&& port == o.port
		&& compass_port == o.compass_port;
	}

	std::string name;
	std::string port;
	std::string compass_port;
};

struct Node {
	Node(const NodeId& n = NodeId(), const Attributes& attr = Attributes())
		: name(n), attributes(attr) {}

	NodeId name;
	Attributes attributes;
};

struct Edge {
	Edge(const NodeId& s, const NodeId& t, const Attributes& attr = Attributes())
	: source(s), target(t), attributes(attr) { }
	NodeId source;
	NodeId target;
	Attributes attributes;
};

struct Graph {
	Graph() : isDirected(false) {}
	bool hasNode(const NodeId& id) const;

	Attributes graph_attributes;
	Attributes node_attributes;
	Attributes edge_attributes;

	std::vector<Node> nodes;
	std::vector<Edge> edges;

	bool isDirected;
	std::string name;
};

/**
 * This is a parser for the DOT graph language as specified under
 * http://www.graphviz.org/doc/info/lang.html
 *
 * It currently does not support subgraphs.
 *
 */
class Reader {
public:
	/**
	 * Parse a stream of characters.
	 *
	 * @return true if parsing was successful, false otherwise.
	 */
	bool parse(const char* begin, const char* end);

	/**
	 * Returned the parsed graph.
	 *
	 * The graph will be empty if parsing was unsuccessful.
	 */
	const Graph& getGraph() const;

private:
	Graph network_;
	bool postProcess_();
};

} // namespace Dot

#endif //DOT_READER_H
