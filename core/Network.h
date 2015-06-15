#ifndef NETWORK_H
#define NETWORK_H

#include "Node.h"
#include <map>

class Network{
	public:
		/**Network Constructor
		 *
		 * @return A Network object
		 *
		 * Constructs an empty network object
		 */
		Network();

		/**readNetwork
		 *
		 * @param filename File containing the structural information of the network
		 *
		 * Depending on the type of the file to read, this method calls the
		 * appropriate reading methods. SIF, NA and TGF files are supported.
		 */
		void readNetwork(const std::string& filename);

		/**cutParents 
		 *
		 * @param id Identifier of the node upon which the parent removal operation
		 * should be performed
		 *
		 * Removes the parental edges of the node with the given identifier.
		 */
		void cutParents(unsigned int id);

		/**cutParents
		 *
		 * @param name Name of the node upon which the parent removal operation
		 * should be performed
		 *
		 * Removes the parental edges  of the node with given name 
		 */
		void cutParents(const std::string& name);

		/**addEdge 
		 *
		 * @param id1 Identifier of the source node for the edge
		 * @param id2 Identifier of the target node for the edge
		 *
		 * This method adds an edge between the nodes id1 and id2. 
		 */
		void addEdge(unsigned int id1, unsigned int id2);

		/**addEdge 
		 *
		 * @param name1 Name of the source node for the edge
		 * @param name2 Name of the target node for the edge
		 *
		 * This method adds an edge between the nodes name1 and name2
		 */
		void addEdge(const std::string& name1, const std::string& name2);
		
		/**removeEdge 
		 *
		 * @param id1 Identifier of the source node for the edge
		 * @param id2 Identifier of the target node for the edge
		 *
		 * This method removes the edge from node id1 to id2
		 */
		void removeEdge(unsigned int id1, unsigned int id2);

		/**removeEdge 
		 *
		 * @param name1 Name of the source node for the edge
		 * @param name2 Name of the target node for the edge
		 *
		 * This method removes the edge from name1 to name2
		 */
		void removeEdge(const std::string& name1, const std::string& name2);
		
		/**setAllUnvisited 
		 *
		 * Sets all nodes in the network to be unvisited
		 */
		void setAllUnvisited();

		/**performDFS 
		 *
		 * @param id Identifier of the DFS start node
		 * @param visitedNodes Vector containing all visited nodes
		 *
		 * Performs Depth First Search in the network starting from the given node.
		 */
		void performDFS(unsigned int id, std::vector<unsigned int>& visitedNodes);

		/**cycleCheck 
		 *
		 * @param sourceID Identifier of the cycle start node
		 * @param currentID Identifier of the current node
		 * @param result Variable holding the result whether or not there is a cycle
		 *
		 * Performs Depth First Search in the network starting from the given node.
		 */
		void cycleCheck(unsigned int sourceID, unsigned int currentID, bool& result);

		/**checkCycleExistence
		 *
		 * @param id Identifier of the cycle start node
		 * @return true if there is a cycle, false otherwise
		 *
		 * Checks if there is a cycle so that this node can be reached from itself.
		 */
		bool checkCycleExistence(unsigned int id);

		/**getNode 
		 *
		 * @param id Identifier of the node of interest
		 *
		 * @return A reference to the desired node
		 *
		 * Getter for a distinct node
		 */
		Node& getNode(unsigned int id) ;

		/**getNode 
		 *
		 * @param name Name of the node of interest
		 *
		 * @return A reference to the desired node
		 *
		 * Getter for a distinct node
		 */
		Node& getNode(const std::string& name);

		/**getNode
		 *
		 * @param id Identifier of the node of interest
		 *
		 * @return A const reference to the desired node
		 *
		 * Const getter for a distince node
		 */
		const Node& getNode(const unsigned int id) const;

		/**getNode 
		 *
		 * @param name Name of the node of interest
		 *
		 * @return A const reference to the desired node
		 *
		 * Const getter for a distinct node
		 */
		const Node& getNode(const std::string& name) const;

		/**hasNode 
		 *
		 * @param name Name of the Node of interest
		  *
		 * @return True if the query node is contained in the network, false otherwise
		 *
		 * Checks whether a node is contained in a network or not
		 */
		bool hasNode(const std::string& name) const;

		/**hasValue 
		 *
		 * @param name Name of the Node of interest
		 * @param value Value of interest
		 *
		 * @return True if the given Node contains the query value, false otherwise
		 *
		 * Checks whether a given node contains a certain value
		 */
		bool hasValue(const std::string& name, const std::string& value) const ;

		/**getNodes 
		 *
		 * @return A reference to the node list of the nework
		 *
		 * Getter for all nodes in the network
		 */
		std::vector<Node>& getNodes();

		/**getNodes 
		 *
		 * @return A const reference to all nodes in the network
		 *
		 * Getter for all nodes in the network
		 */
		const std::vector<Node>& getNodes() const;

		/**getIndex 
		 *
		 * @param name Name of the node of interest
		 *
		 * @return Index of the given Node
		 *
		 * Returns the position of the given node in the nodelist of the network object
		 */
		unsigned int getIndex(const std::string& name) const;

		/**operator<< 
		 *
		 * @param os An ostream object
		 * @param n A network object
		 *
		 * @return A reference to the ostream object
		 *
		 * Standard ostream operator. 
		 */
		friend std::ostream& operator<<(std::ostream& os,const Network& n);

		/**getObservationsMap 
		 *
		 * @return A reference to the ObservationsMap
		 * 
		 */
		std::unordered_map<std::string,int>& getObservationsMap();

		/**getObservationsMapR 
		 *
		 * @return A reference to the ObservationsMapR
		 * 
		 */
		std::map<std::pair<int,int>, std::string>& getObservationsMapR();

		/**getObservationsMap const 
		 *
		 * @return A const reference to the ObservationsMap
		 *
		 */
		const std::unordered_map<std::string,int>& getObservationsMap() const;

		/**getObservationsMapR const 
		 *
		 * @return A const reference to the ObservationsMapR
		 *
		 */
		const std::map<std::pair<int,int>, std::string>& getObservationsMapR() const;

		/**createBackup 
		 *
		 * Creates a Backup of the network structure. Thus, the original network
		 * can be recreated after interventions (Do-Interventions, Additions,
		 * Deletions of edges) have been performed
		 */
		void createBackup();

		/**loadBackup 
		 *
		 * Recreates the network structure from a previously stored network.
		 */
		void loadBackup();

		/**computeFactor 
		 *
		 * @param n Reference to the node of interest
		 *
		 * Calculates the factors to be used for calculating a dense integer
		 * representation of the node values. 
		 */
		void computeFactor(Node& n) const ;

		/**reverseFactor 
		 *
		 * @param n A reference to the node of interest
		 * @param position of the parent in question in the parent list of n
		 * @param row Row of the probability matrix in which the factor occures
		 *
		 * @return The original value of the given node
		 *
		 * Calculates the original value of a node given the dense integer
		 * representation used for internal storage
		 */
		int reverseFactor(Node& n, unsigned int i, int row);

		/**size 
		 *
		 * @return Returns the number of nodes in network
		 *
		 */
		size_t size() const;

		/**saveParameters 
		 *
		 * Creates a file containig all probabilities of the nodes in the network
		 * The filename consists of the current data&time
		 */
		void saveParameters() const;

		/**clearDynProgMatrices 
		 *
		 * Clear all entries in the matrices created for DynamicProgramming
		 */
		void clearDynProgMatrices();

		/**createTwinNetwork
		 *
		 * Creates a TwinNetwork Representation to compute CounterFactualQueries
		 */
		void createTwinNetwork();

		/**removeHypoNodes
	 	 *
	 	 * Removes the Hypothetical Nodes from the nodelist
		 */
		void removeHypoNodes();

		/**getNewID 
		 *
		 * @param originalID, The original ID used in the network file
		 *
		 * @return The corresponding newID
		 *
		 * Given the original Node ID, the corresponding new one is retrieved
		 * and returned
		 */
		unsigned int getNewID(unsigned int originalID);

		/**getHypoStart
		 *
		 * @return Returns the value of the private variable hypostart_
		 */
		unsigned int getHypoStart();

		/**getHypoID
		 *
		 * @param originalID, THe identifier of the original real world node
		 *
		 * @return the hypothetical node identifier
		 */
		unsigned int getHypoID(unsigned int originalID);
		/**reset
		 *
		 * Calls the reset function of all nodes in the NodeList_
		 */
		void reset();

	private:
		/**getParents 
		 *
		 * @param id Identifier of the node of interest
		 *
		 * @return A const vector containig the identifiers of the nodes in question
		 *
		 * This method returns the parents of the node with the given name and
		 * stores those parents in the node itself. Hence this method is private and
		 * is only used for initilisation of the parents field within the Node class
		 */
		const std::vector<unsigned int> getParents(unsigned int id) const;

		/**getParents 
		 *
		 * @param name Name of the node of interest
		 *
		 * @return A const vector containig the identifiers of the nodes in question
		 *
		 * This method returns the parents of the node with the given name and
		 * stores those parents in the node itself. Hence this method is private and
		 * is only used for initilisation of the parents field within the Node class
		 */
		const std::vector<unsigned int> getParents(const std::string& name) const;

		/**getParents 
		 *
		 * @param n A const reference to the node of interest
		 *
		 * @return A const vector containig the identifiers of the nodes in question
		 *
		 * This method returns the parents of given node and
		 * stores those parents in the node itself. Hence this method is private and
		 * is only used for initilisation of the parents field within the Node class 
		 */
		const std::vector<unsigned int> getParents(const Node& n) const;		

		/**getDenseNodeIdentifier 
		 *
		 * @param originalIdentifier The identifier from the network file
		 *
		 * @return A dense node identifier
		 *
		 * Creates a dense node identifier, given the original one
		 */
		size_t getDenseNodeIdentifier(unsigned int originialIdentifier);

		/**assignParents 
		 *
		 * Calls the private getParents method in this class for every Node
		 * and thereby assign the correct parents to each of them. 
		 */
		void assignParents();

	
		/**readTGF 
		 *
		 * @param filename Name of the TGF file
		 *
		 * Reads a TGF file
		 */
		void readTGF(const std::string& filename);

		/**readSIF 
		 *
		 * @param filename Name of the SIF file
		 *
		 * @return
		 *
		 * Reads a SIF file, a NA file must be read in before
		 */
		void readSIF(const std::string& filename);

		/**readNA 
		 *
		 * @param filename Name of the NA file
		 *
		 * Reads a NA file
		 */
		void readNA(const std::string& filename);

		//Matrices to store the network structure
		Matrix<unsigned int> AdjacencyMatrix_;
		Matrix<unsigned int> AdjacencyMatrixBackup_;
		//Maps the identifier of a node to its index in the NodeList_
		std::unordered_map<unsigned int, unsigned int> IDToIndex_;
		//Maps the name of a node to its index int the NodeList_
		std::unordered_map<std::string, unsigned int> NameToIndex_;
		//Depending on the file typ, an unsigned int is returned to allow for switching
		std::unordered_map<std::string, unsigned int> ExtensionToIndex_;
		//Maps the original name of an observation to the internal integer representation
		std::unordered_map<std::string,int> observationsMap_;
		//Maps the original identifiers to the internally used dense identifiers
		std::vector<std::pair<unsigned int, unsigned int>> originalIDToDense_;
		//Maps the internal integer value representation to the original values
		std::map<std::pair<int,int>, std::string> observationsMapR_;
		//Stores the nodes of the network
		std::vector<Node> NodeList_;
		//Stores the startindex of the hypothetical nodes
		unsigned int hypostart_;
		//Mapes the original Node ID to the hypothetical node ID
		std::vector<unsigned int> IDMap_;
};
#endif
