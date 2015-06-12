#include "gtest/gtest.h"
#include "../core/DotReader.h"
#include "config.h"

#include <fstream>
#include <string>

inline std::ostream& operator<<(std::ostream& strm, const Dot::NodeId& id) {
	strm << "'" << id.name << "' '" << id.port << "' '" << id.compass_port << "'";

	return strm;
}

class DotReaderTest : public ::testing::Test {
	protected:
		std::string readFile(const std::string& fileName) {
			std::ifstream input(fileName);
			return std::string{std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
		}

		void assertHasAttribute(const Dot::Attributes& attr, const std::string& key, const std::string& value) {
			auto it = attr.find(key);
			ASSERT_NE(attr.end(), it);
			EXPECT_EQ(value, it->second);
		}
};

TEST_F(DotReaderTest, readSimpleGraph) {
	std::string file = readFile(TEST_DATA_PATH("simpleGraph.dot"));
	Dot::Reader reader;
	ASSERT_TRUE(reader.parse(file.data(), file.data() + file.size()));

	const auto& graph = reader.getGraph();
	EXPECT_FALSE(graph.isDirected);

	EXPECT_EQ("G", graph.name);
	EXPECT_TRUE(graph.node_attributes.empty());
	EXPECT_TRUE(graph.edge_attributes.empty());

	EXPECT_EQ(1, graph.graph_attributes.size());
	assertHasAttribute(graph.graph_attributes, "width", "100.2");

	EXPECT_EQ(2, graph.nodes.size());

	EXPECT_EQ(Dot::NodeId("u"), graph.nodes[0].name);
	EXPECT_EQ(1, graph.nodes[0].attributes.size());
	assertHasAttribute(graph.nodes[0].attributes, "label", "Node 1");

	EXPECT_EQ(Dot::NodeId("v"), graph.nodes[1].name);
	EXPECT_EQ(1, graph.nodes[1].attributes.size());
	assertHasAttribute(graph.nodes[1].attributes, "label", "Node 2");

	EXPECT_EQ(1, graph.edges.size());
	EXPECT_EQ(1, graph.edges[0].attributes.size());
	assertHasAttribute(graph.edges[0].attributes, "color", "blue");
}

TEST_F(DotReaderTest, compassSpecifier) {
	std::string file = readFile(TEST_DATA_PATH("compassGraph.dot"));
	Dot::Reader reader;
	ASSERT_TRUE(reader.parse(file.data(), file.data() + file.size()));

	const auto& graph = reader.getGraph();
	EXPECT_TRUE(graph.isDirected);
	EXPECT_EQ("", graph.name);

	ASSERT_EQ(3, graph.nodes.size());
	ASSERT_EQ(1, graph.edges.size());

	EXPECT_TRUE(graph.node_attributes.empty());
	EXPECT_TRUE(graph.edge_attributes.empty());
	EXPECT_TRUE(graph.graph_attributes.empty());

	EXPECT_EQ(Dot::NodeId("n", "", "ne"), graph.nodes[0].name);
	EXPECT_TRUE(graph.nodes[0].attributes.empty());

	EXPECT_EQ(Dot::NodeId("s", "sese"), graph.nodes[1].name);
	EXPECT_TRUE(graph.nodes[1].attributes.empty());

	EXPECT_EQ(Dot::NodeId("e"), graph.nodes[2].name);
	EXPECT_TRUE(graph.nodes[2].attributes.empty());

	EXPECT_EQ(Dot::NodeId("n"), graph.edges[0].source);
	EXPECT_EQ(Dot::NodeId("e"), graph.edges[0].target);
	EXPECT_TRUE(graph.edges[0].attributes.empty());
}

TEST_F(DotReaderTest, attributeLists) {
	std::string file = readFile(TEST_DATA_PATH("attributeLists.dot"));
	Dot::Reader reader;
	ASSERT_TRUE(reader.parse(file.data(), file.data() + file.size()));

	const auto& graph = reader.getGraph();
	EXPECT_TRUE(graph.isDirected);

	EXPECT_EQ("SomeGraph", graph.name);
	EXPECT_TRUE(graph.nodes.empty());
	EXPECT_TRUE(graph.edges.empty());
	EXPECT_TRUE(graph.edge_attributes.empty());

	EXPECT_EQ(3, graph.graph_attributes.size());
	assertHasAttribute(graph.graph_attributes, "bla", "blubb");
	assertHasAttribute(graph.graph_attributes, "heyHo12", "haha");
	assertHasAttribute(graph.graph_attributes, "0.1", "3.4");

	EXPECT_EQ(3, graph.node_attributes.size());
	assertHasAttribute(graph.node_attributes, "color", "blue");
	assertHasAttribute(graph.node_attributes, "dumb", "foo");
	assertHasAttribute(graph.node_attributes, "bla", "foobar");
}

TEST_F(DotReaderTest, edgeList) {
	std::string file = readFile(TEST_DATA_PATH("edgeList.dot"));
	Dot::Reader reader;
	ASSERT_TRUE(reader.parse(file.data(), file.data() + file.size()));

	const auto& graph = reader.getGraph();
	EXPECT_TRUE(graph.edge_attributes.empty());
	EXPECT_TRUE(graph.node_attributes.empty());
	EXPECT_TRUE(graph.graph_attributes.empty());

	EXPECT_EQ(3, graph.edges.size());
	EXPECT_EQ(Dot::NodeId("a"), graph.edges[0].source);
	EXPECT_EQ(Dot::NodeId("b"), graph.edges[0].target);
	EXPECT_EQ(Dot::NodeId("b"), graph.edges[1].source);
	EXPECT_EQ(Dot::NodeId("c"), graph.edges[1].target);
	EXPECT_EQ(Dot::NodeId("c"), graph.edges[2].source);
	EXPECT_EQ(Dot::NodeId("d"), graph.edges[2].target);

	assertHasAttribute(graph.edges[0].attributes, "color", "red");
	assertHasAttribute(graph.edges[1].attributes, "color", "red");
	assertHasAttribute(graph.edges[2].attributes, "color", "red");
}