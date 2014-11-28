#ifndef NETWORK_H
#define NETWORK_H

#include "Node.h"
#include <map>
#include <boost/filesystem.hpp>

class Network{
	public:
		Network();
		void readNetwork(const std::string& filename);
		void cutParents(unsigned int id);
		void cutParents(const std::string& name);
		void addEdge(unsigned int id1, unsigned int id2);
		void addEdge(const std::string& name1, const std::string& name2);
		void removeEdge(unsigned int id1, unsigned int id2);
		void removeEdge(const std::string& name1, const std::string& name2);
		void setAllUnvisited();
		void performDFS(unsigned int id, std::vector<unsigned int>& visitedNodes);
		Node& getNode(unsigned int id) ;
		Node& getNode(const std::string& name);
		const Node& getNode(const unsigned int id) const;
		const Node& getNode(const std::string& name) const;
		const bool hasNode(const std::string& name) const;
		const bool hasValue(const std::string& name, const std::string& value) const ;
		std::vector<Node>& getNodes();
		std::vector<unsigned int> getNodeIDs();
		const std::vector<Node>& getNodes() const;
		const std::vector<unsigned int> getNodeIDs() const;
		const unsigned int getIndex(unsigned int id) const ;
		const unsigned int getIndex(const std::string& name) const;
		friend std::ostream& operator<<(std::ostream& os,const Network& n);
		std::unordered_map<std::string,int>& getObservationsMap();
		std::map<std::pair<int,int>, std::string>& getObservationsMapR();
		const std::unordered_map<std::string,int>& getObservationsMap() const;
		const std::map<std::pair<int,int>, std::string>& getObservationsMapR() const;
		void createBackup();
		void loadBackup();
		const int computeFactor(const Node& n, unsigned int parentID) const ;
		const int reverseFactor(const Node& n, unsigned int parentID, int row) const;
		unsigned int getNewID(unsigned int originalID);
		const unsigned int size() const;
	private:
		const std::vector<unsigned int> getParents(unsigned int id) const;
		const std::vector<unsigned int> getParents(const std::string& name) const;
		const std::vector<unsigned int> getParents(Node& n) const;
		const std::vector<unsigned int> getParents(const Node& n) const;		
		unsigned int getDenseNodeIdentifier(unsigned int originialIdentifier);
		void assignParents();
		void readTGF(const std::string& filename);
		void readSIF(const std::string& filename);
		void readNA(const std::string& filename);
		Matrix<unsigned int> AdjacencyMatrix_;
		Matrix<unsigned int> AdjacencyMatrixBackup_;
		std::unordered_map<unsigned int, unsigned int> IDToIndex_;
		std::unordered_map<std::string, unsigned int> NameToIndex_;
		std::unordered_map<std::string, unsigned int> ExtensionToIndex_;
		std::unordered_map<std::string,int> observationsMap_;
		std::vector<std::pair<unsigned int, unsigned int>> originalIDToDense_;
		std::map<std::pair<int,int>, std::string> observationsMapR_;
		std::vector<Node> NodeList_;
};
#endif
