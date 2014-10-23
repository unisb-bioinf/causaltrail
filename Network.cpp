#include "Network.h"

Network::Network(std::string filename)
	:AdjacencyMatrix_(Matrix<unsigned int>(0,0,0)){
	readNetwork(filename);
	}

unsigned int Network::getIndex(unsigned int id){
	return IDToIndex_[id];
	}

unsigned int Network::getIndex(std::string name){
	return NameToIndex_[name];
	}

std::vector<unsigned int> Network::getParents(unsigned int id){
	std::vector<unsigned int> parentIDs;
	unsigned int index=getIndex(id);
	for (int row=0; row<AdjacencyMatrix_.getRowCount(); row++){
			if (AdjacencyMatrix_(index,row)==1){
				unsigned int temp;
				std::stringstream ss;
				ss<<AdjacencyMatrix_.getRowNames()[row];
				ss >>temp;
				parentIDs.push_back(temp); 
				}
		}
	return parentIDs;
	}

std::vector<unsigned int> Network::getParents(std::string name){
	return getParents(getNode(name).getID());
	}

void Network::cutParents(unsigned int id){
	std::vector<unsigned int> parentIDs=getParents(id);
	unsigned int index=getIndex(id);
	for (auto n:parentIDs){
		AdjacencyMatrix_.setData(0,getIndex(n),index);
		}
	}

void Network::cutParents(std::string name){
	cutParents(getNode(name).getID());
	}

void Network::addEdge(unsigned int id1, unsigned int id2){
	AdjacencyMatrix_.setData(1,getIndex(id1),getIndex(id2));
	}

void Network::addEdge(std::string name1, std::string name2){
	addEdge(getNode(name1).getID(),getNode(name2).getID());
	}

void Network::removeEdge(unsigned int id1, unsigned int id2){
	AdjacencyMatrix_.setData(0,getIndex(id1),getIndex(id2));
	}

void Network::removeEdge(std::string name1, std::string name2){
	removeEdge(getNode(name1).getID(),getNode(name2).getID());
	}

Node& Network::getNode(unsigned int id){
	return NodeList_[getIndex(id)];
	}

Node& Network::getNode(std::string name){
	return NodeList_[getIndex(name)];
	}

std::ostream& operator<<(std::ostream& os, const Network& n){
	return os<<n.AdjacencyMatrix_;
	}

void Network::readNetwork(std::string filename){
	readTGF(filename);
	}

void Network::readTGF(std::string filename){
	NodeList_.clear();
	std::string line;
	std::ifstream input(filename,std::ifstream::in);
	unsigned int id1;
	unsigned int id2;
	std::string name;
	unsigned int index=0;
	std::vector<std::string> names;
	while(std::getline(input,line)){
		if (line == "#")
			break;
		std::stringstream buffer;
		buffer<<line;
		buffer>>id1>>name;
		NodeList_.push_back(Node(0,id1,name));
		IDToIndex_[id1]=index;
		NameToIndex_[name]=index;
		std::stringstream nbuffer;
		nbuffer<<id1;
		std::string ids;
		nbuffer>>ids;
		names.push_back(ids);
		index++;	
	}
	AdjacencyMatrix_.resize(NodeList_.size(),NodeList_.size(),0);
	AdjacencyMatrix_.setRowNames(names);
	AdjacencyMatrix_.setColNames(names);
	while(std::getline(input,line)){
		std::stringstream buffer;
		buffer<<line;
		buffer>>id1>>id2;
		addEdge(id2,id1);
	}
	input.close();
	}

void Network::readSIF(std::string filename){
 	std::string line;
	unsigned int id1;
	unsigned int id2;
	std::string relation;
 	std::ifstream input(filename,std::ifstream::in);
 	if (NodeList_.empty())
		throw std::invalid_argument("You have to read in a .na file beforehand.");	
	while(std::getline(input,line)){
		std::stringstream buffer;
		buffer<<line;
		buffer>>id1>>relation>>id2;
		addEdge(id2,id1);
		}	
	input.close();	
}

void Network::readNA(std::string filename){
	NodeList_.clear();
	std::string line;
	std::ifstream input(filename,std::ifstream::in);
	unsigned int id1;
	std::string name;
	std::string ids;
	unsigned int index=0;
	std::vector<std::string> names;
	while(std::getline(input,line)){
		std::stringstream buffer;
		buffer<<line;
		buffer>>id1>>name;
		NodeList_.push_back(Node(0,id1,name));
		IDToIndex_[id1]=index;
		NameToIndex_[name]=index;
		std::stringstream nbuffer;
		nbuffer<<id1;
		nbuffer>>ids;
		names.push_back(ids);
		index++;
	}	
	input.close();
	AdjacencyMatrix_.resize(NodeList_.size(),NodeList_.size(),0);
	AdjacencyMatrix_.setRowNames(names);
	AdjacencyMatrix_.setColNames(names);
	}
