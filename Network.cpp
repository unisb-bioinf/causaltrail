#include "Network.h"
#include <ctime>
#include <chrono>
#include <fstream>
#include <iostream>

Network::Network()
    : AdjacencyMatrix_(Matrix<unsigned int>(0, 0, 0)),
      AdjacencyMatrixBackup_(Matrix<unsigned int>(0, 0, 0))
{
	ExtensionToIndex_[".tgf"] = 1;
	ExtensionToIndex_[".na"] = 2;
	ExtensionToIndex_[".sif"] = 3;
}

struct Comp {
    bool operator()(std::pair<unsigned int, unsigned int> p, unsigned int s) const
    { return p.first < s; }
    bool operator()(unsigned int s, std::pair<unsigned int, unsigned int> p) const
    { return s < p.first; }
    bool operator()(std::pair<unsigned int, unsigned int> p1,std::pair<unsigned int,unsigned int> p2) const
    { return p1.first < p2.first; }
  
};



const unsigned int Network::getIndex(unsigned int id) const
{
	auto res = IDToIndex_.find(id);
	if(res == IDToIndex_.end()) {
		throw std::invalid_argument("Identifier not found");
	}
	return res->second;
}


const unsigned int Network::getIndex(const std::string& name) const
{
	auto res = NameToIndex_.find(name);
	if(res == NameToIndex_.end()) {
		throw std::invalid_argument("Identifier not found");
	}
	return res->second;
}

const std::vector<unsigned int> Network::getParents(unsigned int id) const
{
	std::vector<unsigned int> parentIDs;
	unsigned int index = getIndex(id);
	for(unsigned int row = 0; row < AdjacencyMatrix_.getRowCount(); row++) {
		if(AdjacencyMatrix_(index, row) == 1) {
			unsigned int temp;
			std::stringstream ss;
			ss << AdjacencyMatrix_.getRowNames()[row];
			ss >> temp;
			parentIDs.push_back(temp);
		}
	}
	return parentIDs;
}

const std::vector<unsigned int> Network::getParents(Node& n) const
{
	return getParents(n.getID());
}

const std::vector<unsigned int> Network::getParents(const Node& n) const
{
	return getParents(n.getID());
}

const std::vector<unsigned int> Network::getParents(const std::string& name)
    const
{
	return getParents(getNode(name).getID());
}

std::vector<Node>& Network::getNodes() { return NodeList_; }

std::vector<unsigned int> Network::getNodeIDs()
{
	std::vector<unsigned int> temp;
	for(const auto& n : NodeList_) {
		temp.push_back(n.getID());
	}
	return temp;
}

const std::vector<Node>& Network::getNodes() const { return NodeList_; }

const std::vector<unsigned int> Network::getNodeIDs() const
{
	std::vector<unsigned int> temp;
	for(const auto& n : NodeList_) {
		temp.push_back(n.getID());
	}
	return temp;
}

void Network::cutParents(unsigned int id)
{
	std::vector<unsigned int> parentIDs = getParents(id);
	unsigned int index = getIndex(id);
	for(auto& n : parentIDs) {
		AdjacencyMatrix_.setData(0, getIndex(n), index);
	}
	getNode(id).cutParents();
}

void Network::cutParents(const std::string& name)
{
	cutParents(getNode(name).getID());
}

void Network::addEdge(unsigned int id1, unsigned int id2)
{
	AdjacencyMatrix_.setData(1, getIndex(id1), getIndex(id2));
	getNode(id1).setParents(getParents(id1));
}

void Network::addEdge(const std::string& name1, const std::string& name2)
{
	addEdge(getNode(name1).getID(), getNode(name2).getID());
}


void Network::removeEdge(unsigned int id1, unsigned int id2)
{
	AdjacencyMatrix_.setData(0, getIndex(id1), getIndex(id2));
	getNode(id1).setParents(getParents(id1));
}

void Network::removeEdge(const std::string& name1, const std::string& name2)
{
	removeEdge(getNode(name1).getID(), getNode(name2).getID());
}

Node& Network::getNode(unsigned int id) { return NodeList_[getIndex(id)]; }

const Node& Network::getNode(const unsigned int id) const
{
	return NodeList_[getIndex(id)];
}

Node& Network::getNode(const std::string& name)
{
	return NodeList_[getIndex(name)];
}

const Node& Network::getNode(const std::string& name) const
{
	return NodeList_[getIndex(name)];
}

std::ostream& operator<<(std::ostream& os, const Network& n)
{
	os << n.AdjacencyMatrix_ << "\n";
	for(const Node& node : n.NodeList_) {
		os << node;
	}
	return os;
}

void Network::readNetwork(const std::string& filename)
{
	std::string extension = boost::filesystem::extension(filename);
	switch(ExtensionToIndex_[extension]) {
		case 1:
			readTGF(filename);
			break;
		case 2:
			readNA(filename);
			break;
		case 3:
			readSIF(filename);
			break;
		default:
			throw std::invalid_argument("Unsupported file type");
	}
	assignParents();
}

void Network::readTGF(const std::string& filename)
{
	NodeList_.clear();
	AdjacencyMatrix_.clear();
	std::string line;
	std::ifstream input(filename, std::ifstream::in);
	if (! input.good()){
		throw std::invalid_argument("File not found");
		}
	unsigned int id1;
	unsigned int id2;
	std::string name;
	unsigned int index = 0;
	std::vector<std::string> names;
	while(std::getline(input, line)) {
		if(line == "#")
			break;
		std::stringstream buffer;
		buffer << line;
		buffer >> id1 >> name;
		id1=getDenseNodeIdentifier(id1);
		NodeList_.push_back(Node(0, id1, name));
		IDToIndex_[id1] = index;
		NameToIndex_[name] = index;
		std::stringstream nbuffer;
		nbuffer << id1;
		std::string ids;
		nbuffer >> ids;
		names.push_back(ids);
		index++;
	}
	AdjacencyMatrix_.resize(NodeList_.size(), NodeList_.size(), 0);
	AdjacencyMatrix_.setRowNames(names);
	AdjacencyMatrix_.setColNames(names);
	std::sort(originalIDToDense_.begin(), originalIDToDense_.end(),Comp());
	bool edges = false;
	while(std::getline(input, line)) {
		edges=true;
		std::stringstream buffer;
		buffer << line;
		buffer >> id1 >> id2;
		addEdge(getNewID(id2),getNewID(id1));
	}
	if (!edges){
		throw std::invalid_argument("No edges read from file, either # is missing, or there no edges encoded");
	}
	input.close();
}

void Network::readSIF(const std::string& filename)
{
	std::string line;
	unsigned int id1;
	unsigned int id2;
	std::string relation;
	std::ifstream input(filename, std::ifstream::in);
	if (! input.good()){
		throw std::invalid_argument("File not found");
		}
	if(NodeList_.empty())
		throw std::invalid_argument(
		    "You have to read in a .na file beforehand.");
	while(std::getline(input, line)) {
		std::stringstream buffer;
		buffer << line;
		buffer >> id1 >> relation >> id2;
		if (relation == ""){
			throw std::invalid_argument("Invalid file structure of sif file");
		}
		addEdge(getNewID(id2), getNewID(id1));
		id2 = -1;
		id1 = -1;
		relation = "";
	}
	input.close();
}

void Network::readNA(const std::string& filename)
{
	NodeList_.clear();
	AdjacencyMatrix_.clear();
	std::string line;
	std::ifstream input(filename, std::ifstream::in);
	if (! input.good()){
		throw std::invalid_argument("File not found");
		}
	unsigned int id1;
	std::string name = "";
	std::string waste = "";
	std::string ids;
	unsigned int index = 0;
	std::vector<std::string> names;
	std::getline(input, line);
	while(std::getline(input, line)) {
		std::stringstream buffer;
		buffer << line;
		buffer >> id1 >> waste >> name;
		if ((waste == "") or (name == "")){
			throw std::invalid_argument("Invalid structure of na file");
		}
		id1=getDenseNodeIdentifier(id1);
		NodeList_.push_back(Node(0, id1, name));
		IDToIndex_[id1] = index;
		NameToIndex_[name] = index;
		std::stringstream nbuffer;
		nbuffer << id1;
		nbuffer >> ids;
		names.push_back(ids);
		index++;
		waste = "";
		name = "";
	}
	input.close();
	AdjacencyMatrix_.resize(NodeList_.size(), NodeList_.size(), 0);
	AdjacencyMatrix_.setRowNames(names);
	AdjacencyMatrix_.setColNames(names);
	std::sort(originalIDToDense_.begin(), originalIDToDense_.end(),Comp());
}

void Network::assignParents()
{
	for(auto& n : NodeList_) {
		n.setParents(getParents(n));
	}
}

std::unordered_map<std::string, int>& Network::getObservationsMap()
{
	return observationsMap_;
}
std::map<std::pair<int, int>, std::string>& Network::getObservationsMapR()
{
	return observationsMapR_;
}

const std::unordered_map<std::string, int>& Network::getObservationsMap() const
{
	return observationsMap_;
}
const std::map<std::pair<int, int>, std::string>& Network::getObservationsMapR()
    const
{
	return observationsMapR_;
}

void Network::setAllUnvisited()
{
	for(auto& n : NodeList_) {
		n.setUnvisited();
	}
}

void Network::performDFS(unsigned int id,
                         std::vector<unsigned int>& visitedNodes)
{
	Node& n = getNode(id);
	if(not n.isVisited()) {
		n.visit();
		visitedNodes.push_back(n.getID());
		for(int pid : n.getParents()) {
			performDFS(pid, visitedNodes);
		}
	}
}

void Network::createBackup() { AdjacencyMatrixBackup_ = AdjacencyMatrix_; }

void Network::loadBackup()
{
	AdjacencyMatrix_ = AdjacencyMatrixBackup_;
	AdjacencyMatrixBackup_ = Matrix<unsigned int>(0, 0, 0);
}

const int Network::computeFactor(const Node& n, unsigned int parentID) const
{
	bool flag = false;
	int factor = 1;
	for(const auto& key : n.getParents()) {
		if(flag) {
			factor *= getNode(key).getUniqueValuesExcludingNA().size();
		}
		if(key == parentID) {
			flag = true;
		}
	}
	return factor;
}

const int Network::reverseFactor(const Node& n, unsigned int parentID, int row)
    const
{
	int value = row;
	for(const auto& key : n.getParents()) {
		int factor = computeFactor(n, key);
		int result = value / factor;
		value = (value % computeFactor(n, key));
		if(key == parentID) {
			return result;
		}
	}
	return -1;
}

const bool Network::hasNode(const std::string& name) const
{
	auto res = NameToIndex_.find(name);
	if(res == NameToIndex_.end()) {
		return false;
	}
	return true;
}

const bool Network::hasValue(const std::string& nodeName,
                             const std::string& valueName) const
{
	const std::vector<std::string>& valueNames =
	    getNode(nodeName).getValueNamesProb();
	auto it = std::find(valueNames.begin(), valueNames.end(), valueName);
	if(it == valueNames.end())
		return false;
	return true;
}

unsigned int Network::getDenseNodeIdentifier(unsigned int originialIdentifier)
{
	unsigned int newID = NodeList_.size();
	originalIDToDense_.push_back(std::make_pair(originialIdentifier, newID));
	return newID;
}

unsigned int Network::getNewID(unsigned int originalIdentifier)
{
	auto low =
	    std::lower_bound(originalIDToDense_.begin(), originalIDToDense_.end(),
	                     originalIdentifier, Comp());
	if(low != originalIDToDense_.end()) {
		return low->second;
	}
	throw std::invalid_argument("Identifier not found");
}

const unsigned int Network::size() const { return getNodes().size(); }

void Network::saveParameters() const{
	std::chrono::time_point<std::chrono::system_clock> time = std::chrono::system_clock::now();
    std::time_t time_ = std::chrono::system_clock::to_time_t(time);
	std::string ts = std::ctime(&time_);
	std::ofstream file;
	std::string filename = "Parameters_"+ts;
	file.open(filename);
	for (auto& n : NodeList_){
		unsigned int rowCount=n.getNumberOfParentValues();
		unsigned int colCount=n.getNumberOfUniqueValuesExcludingNA();
		for (unsigned int row=0;row<rowCount;row++)
			for (unsigned int col=0;col<colCount;col++)
				file<<n.getProbability(col,row)<<"\n";
		}
	file.close();
}

void Network::clearDynProgMatrices(){
	for (auto& n : NodeList_){
		n.clearDynProgMatrix();
	}
}
