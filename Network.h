#ifndef NETWORK_H
#define NETWORK_H

#include "Node.h"
#include <map>
#include <boost/filesystem.hpp>

class Network{
	public:
		Network();
		void readNetwork(std::string filename);
		void cutParents(unsigned int id);
		void cutParents(std::string name);
		void addEdge(unsigned int id1, unsigned int id2);
		void addEdge(std::string name1, std::string name2);
		void removeEdge(unsigned int id1, unsigned int id2);
		void removeEdge(std::string name1, std::string name2);
		void setAllUnvisited();
		void performDFS(unsigned int id, std::vector<unsigned int>& visitedNodes);
		Node& getNode(unsigned int id);
		Node& getNode(std::string name);
		bool hasNode(std::string name);
		bool hasValue(std::string name, std::string value);
		std::vector<Node>& getNodes();
		std::vector<unsigned int> getNodeIDs();
		unsigned int getIndex(unsigned int id);
		unsigned int getIndex(std::string name);
		friend std::ostream& operator<<(std::ostream& os,const Network& n);
		std::unordered_map<std::string,int>& getObservationsMap();
		std::map<std::pair<int,int>, std::string>& getObservationsMapR();
		void createBackup();
		void loadBackup();
		int computeFactor(const Node& n, int parentID);
		int reverseFactor(const Node& n, int parentID, int row);
	private:
		std::vector<unsigned int> getParents(unsigned int id);
		std::vector<unsigned int> getParents(std::string name);
		std::vector<unsigned int> getParents(Node& n);
		std::vector<unsigned int> getParents(const Node& n);		
		void assignParents();
		void readTGF(std::string filename);
		void readSIF(std::string filename);
		void readNA(std::string filename);
		Matrix<unsigned int> AdjacencyMatrix_;
		Matrix<unsigned int> AdjacencyMatrixBackup_;
		std::unordered_map<unsigned int, unsigned int> IDToIndex_;
		std::unordered_map<std::string, unsigned int> NameToIndex_;
		std::unordered_map<std::string, unsigned int> ExtensionToIndex_;
		std::unordered_map<std::string,int> observationsMap_;
		std::map<std::pair<int,int>, std::string> observationsMapR_;
		std::vector<Node> NodeList_;
};
#endif
