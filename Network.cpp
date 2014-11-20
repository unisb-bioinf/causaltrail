#include "Network.h"

/**Detailed Constructor
 *
 * @return A network object
 *  
 */
Network::Network()
	:AdjacencyMatrix_(Matrix<unsigned int>(0,0,0)),AdjacencyMatrixBackup_(Matrix<unsigned int>(0,0,0)){
	ExtensionToIndex_[".tgf"]=0;
	ExtensionToIndex_[".na"]=1;
	ExtensionToIndex_[".sif"]=2;
	}

/**getIndex
 *
 * @param id identifier of the query node
 *
 * @return index of the query node
 * 
 */
unsigned int Network::getIndex(unsigned int id){
	auto res=IDToIndex_.find(id);
	if (res==IDToIndex_.end()){
		throw std::invalid_argument("Identifier not found");
		}
	return res->second;
	}

/**getIndex
 *
 * @param name name of the query node
 *
 * @return index of the query node
 * 
 */
unsigned int Network::getIndex(std::string name){
	auto res=NameToIndex_.find(name);
	if (res==NameToIndex_.end()){
		throw std::invalid_argument("Identifier not found");
		}
	return res->second;
	}

/**getParents
 *
 * @param id identifier of the query node
 *
 * @return vector containing the identifiers of the parents from the query node
 *  
 */
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

/**getParents
 * 
 * @param Node n
 * 
 * @return vector containing the identifiers of the parents from the query node
 *
 */
std::vector<unsigned int> Network::getParents(Node& n){
	return getParents(n.getID());
	}

/**getParents
 * 
 * @param Node n
 * 
 * @return vector containing the identifiers of the parents from the query node
 *
 */
std::vector<unsigned int> Network::getParents(const Node& n){
	return getParents(n.getID());
	}

/**getParents
 *
 * @param name name of the query node
 *
 * @return vector containing the identifiers of the parents from the query node 
 * 
 */
std::vector<unsigned int> Network::getParents(std::string name){
	return getParents(getNode(name).getID());
	}

/**getNodes
 *
 * @return a reference to the NodeList
 *
 */
std::vector<Node>& Network::getNodes(){
	return NodeList_;
	}

std::vector<unsigned int> Network::getNodeIDs(){
	std::vector<unsigned int> temp;
	for (auto& n: NodeList_){
		temp.push_back(n.getID());
	}
	return temp;
}

/**cutParents
 *
 * @param id identifier of the query node
 *
 * @return void
 * 
 * Removes the edge between the query node and its parents
 */
void Network::cutParents(unsigned int id){
	std::vector<unsigned int> parentIDs=getParents(id);
	unsigned int index=getIndex(id);
	for (auto n:parentIDs){
		AdjacencyMatrix_.setData(0,getIndex(n),index);
		}
	getNode(id).cutParents();
	}

/**cutParents
 *
 * @param id identifier of the query node
 *
 * @return void
 * 
 * Removes the edges between the query node and its parents
 */
void Network::cutParents(std::string name){
	cutParents(getNode(name).getID());
	}

/**addEdge
 *
 * @param id1 identifier of the target node
 * @param id2 identifier of the source node
 *
 * @return void
 * 
 * Adds an edge between the nodes id1 and id2. The edge is directed from id2 to id1.
 */
void Network::addEdge(unsigned int id1, unsigned int id2){
	AdjacencyMatrix_.setData(1,getIndex(id1),getIndex(id2));
	getNode(id1).setParents(getParents(id1));
	}

/**addEdge
 *
 * @param name1 name of the target node
 * @param name2 name of the source node
 *
 * @return void
 * 
 * Adds an edge between the nodes name1 and name2. The edge is directed from node name2 to node name1.
 */
void Network::addEdge(std::string name1, std::string name2){
	addEdge(getNode(name1).getID(),getNode(name2).getID());
	}

/**removeEdge
 *
 * @param id1 identifier of the target node
 * @param id2 identifier of the source node
 *
 * @return void
 * 
 * Removes an edge between the nodes id1 and id2. The edge was directed from id2 to id1.
 */
void Network::removeEdge(unsigned int id1, unsigned int id2){
	AdjacencyMatrix_.setData(0,getIndex(id1),getIndex(id2));
	getNode(id1).setParents(getParents(id1));
	}

/**removeEdge
 *
 * @param name1 name of the target node
 * @param name2 name of the source node
 *
 * @return void
 * 
 * Removes an edge between the nodes name1 and name2. The edge was directed from name2 to name1.
 */
void Network::removeEdge(std::string name1, std::string name2){
	removeEdge(getNode(name1).getID(),getNode(name2).getID());
	}

/**getNode
 *
 * @param id identifier of the query node
 *
 * @return A reference to the fitting node
 * 
 */
Node& Network::getNode(unsigned int id){
	return NodeList_[getIndex(id)];
	}

/**getNode
 *
 * @param name name of the query node
 *
 * @return A reference to the fitting node 
 * 
 */
Node& Network::getNode(std::string name){
	return NodeList_[getIndex(name)];
	}

/**operator<<
 *
 * @param ostream
 * @param n Networb object
 *
 * @return ostream
 * 
 * Prints the AdjacencyMatrix of the network
 */
std::ostream& operator<<(std::ostream& os, const Network& n){
	os<<n.AdjacencyMatrix_<<"\n";
	for (Node node: n.NodeList_){
		os<<node;
		}
	return os;
	}

/**readNetwork
 *
 * @param filename
 *
 * @return void 
 * 
 * Depending on the typ of the inputfile, an other method to read the network is called
 * 
 * @exception invalid_argument if an unsupported file format is detected
 */
void Network::readNetwork(std::string filename){
	std::string extension  = boost::filesystem::extension(filename);
	switch(ExtensionToIndex_[extension]){
		case 0 :readTGF(filename); break;
		case 1 :readNA(filename); break;
		case 2 :readSIF(filename); break;
		default : throw std::invalid_argument("Unsupported file type");
		}
	assignParents();
	}

/**readTGF
 *
 * @param filename
 *
 * @return void 
 * 
 * Reads and stores a network in the trivialgraphformat(TGF)
 */
void Network::readTGF(std::string filename){
	NodeList_.clear();
	AdjacencyMatrix_.clear();
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

/**readSIF
 *
 * @param filename
 *
 * @return void 
 * 
 * Read and stores a network in the SimpleInteractionFormat(SIF). Nodes must be generated before
 * this method is called using readNA().
 *
 * @exception invalid_argument, if there are no Nodes in memory.
 */
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

/**readNA
 *
 * @param filename 
 *
 * @return void 
 * 
 * Reads nodes stored in a Node Attribute file (NA). Must be executed before readSIF().
 */
void Network::readNA(std::string filename){
	NodeList_.clear();
	AdjacencyMatrix_.clear();
	std::string line;
	std::ifstream input(filename,std::ifstream::in);
	unsigned int id1;
	std::string name;
	std::string waste;
	std::string ids;
	unsigned int index=0;
	std::vector<std::string> names;
	std::getline(input,line);
	while(std::getline(input,line)){
		std::stringstream buffer;
		buffer<<line;
		buffer>>id1>>waste>>name;
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

void Network::assignParents(){
	for (auto& n : NodeList_){
		n.setParents(getParents(n));
	}
}

std::unordered_map<std::string,int>& Network::getObservationsMap(){
	return observationsMap_;
}
std::map<std::pair<int,int>, std::string>& Network::getObservationsMapR(){
	return observationsMapR_;
}

void Network::setAllUnvisited(){
	for (auto& n: NodeList_){
		n.setUnvisited();
	}		
}

void Network::performDFS(unsigned int id, std::vector<unsigned int>& visitedNodes){
	Node& n = getNode(id);
	if (not n.isVisited()){
		n.visit();
		visitedNodes.push_back(n.getID());
		for (int pid : n.getParents()){
			performDFS(pid,visitedNodes);
		}
	}	
}

void Network::createBackup(){
	AdjacencyMatrixBackup_=AdjacencyMatrix_;
}

void Network::loadBackup(){
	AdjacencyMatrix_=AdjacencyMatrixBackup_;
	AdjacencyMatrixBackup_=Matrix<unsigned int>(0,0,0);
}

int Network::computeFactor(const Node& n, int parentID){
	bool flag = false;
	int factor = 1;
	for (auto key : n.getParents()){
		if (flag){
			factor*=getNode(key).getUniqueValuesExcludingNA().size();
		}
		if (key==parentID){
			flag=true;
		}
	}
	return factor;
}

int Network::reverseFactor(const Node& n, int parentID, int row){
	int value = row;
	int result = -1;
	bool flag = true;
	for (auto key : n.getParents()){
		int factor = computeFactor(n,key);
		int result = value / factor;
		value = (value % computeFactor(n,key));
		if (key == parentID){
			return result;
		}
	}
	return -1;
}

bool Network::hasNode(std::string name){
	auto res=NameToIndex_.find(name);
	if (res==NameToIndex_.end()){
		return false;
		}
	return true;
	
}

bool Network::hasValue(std::string nodeName, std::string valueName){
	const std::vector<std::string>& valueNames = getNode(nodeName).getValueNamesProb();
	auto it = std::find(valueNames.begin(), valueNames.end(),valueName);
	if (it == valueNames.end())
		return false;
	return true;
	
}
