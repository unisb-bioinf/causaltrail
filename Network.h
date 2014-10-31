#ifndef NETWORK_H
#define NETWORK_H

#include "Node.h"

class Network{
	public:
		Network();
		void readNetwork(std::string filename);
		std::vector<unsigned int> getParents(unsigned int id);
		std::vector<unsigned int> getParents(std::string name);
		std::vector<unsigned int> getParents(Node& n);
		void cutParents(unsigned int id);
		void cutParents(std::string name);
		void addEdge(unsigned int id1, unsigned int id2);
		void addEdge(std::string name1, std::string name2);
		void removeEdge(unsigned int id1, unsigned int id2);
		void removeEdge(std::string name1, std::string name2);
		Node& getNode(unsigned int id);
		Node& getNode(std::string name);
		std::vector<Node>& getNodes();
		unsigned int getIndex(unsigned int id);
		unsigned int getIndex(std::string name);
		friend std::ostream& operator<<(std::ostream& os,const Network& n);
	private:
		void assignParents();
		void readTGF(std::string filename);
		void readSIF(std::string filename);
		void readNA(std::string filename);
		Matrix<unsigned int> AdjacencyMatrix_;
		std::unordered_map<unsigned int, unsigned int> IDToIndex_;
		std::unordered_map<std::string, unsigned int> NameToIndex_;
		std::unordered_map<std::string, unsigned int> ExtensionToIndex_;
		std::vector<Node> NodeList_;
};
#endif
