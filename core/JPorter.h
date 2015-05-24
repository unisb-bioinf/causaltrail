#ifndef JPORTER_H
#define JPORTER_H

#include <boost/property_tree/ptree.hpp>
#include <unordered_map>

class JPorter{
	public:

	/**Constructor
	*
	* Constructs a JPorter object used to import and export json files
	* containing information on data discretisation
	*/
	JPorter();

	/**Deconstructor
	*
	*/
	~JPorter();

	/**importFile
	*
	* @param filename Name of the file containing discretisation information
	*
	* Creates a boost property tree representing the specified json file
	*/
	void importFile(const std::string& filename);

	/**printContent
	*
	* Prints the discretisation information contained in the boost property tree. 
	*/
	void printContent() const;

	/**exportToFile
	*
	* @param filename Name of the json file to be created
	*
	* Exports the current boost property tree to a json file with the given name.
	*/
	void exportToFile(const std::string& filename);

	/**containsNode
	*
	* @param name Name of the node/feature for which existence should be checked
	*
	* @return bool True if the property tree contains a entry for the specified node
	*
	* Checks if the property tree contains an entry for a certain node
	*/
	bool containsNode(const std::string& name) const;

	/**getDiscretisationDetails
	*
	* @param name Name of the node/feature for which discretisation information should be extracted
	*
	* @return std::pair. Its first element is an integer value representing the discretisation method.	
	* The second element contains parameters for the method (not neccessary in all cases)
	*
	* Extractes discretisation information from the property tree for a given node/feature.
	*/
	std::pair<unsigned int, float> getDiscretisationDetails(const std::string& name) const;

	/**addToTree
	*
	* @param name Name of the node/feature to be added
	* @param method Integer encoding of the discretisation method to be added
	*
	* Adds discretisation information to the property tree
	*/
	void addToTree(const std::string& name, unsigned int method);

	/**addToTree
	*
	* @param name Name of the node/feature to be added
	* @param method Integer encoding of the discretisation method to be added
	* @param value Float Parameter for the discretisation method
	*
	* Adds discretisation information to the property tree
	*/
	void addToTree(const std::string& name, unsigned int method, float value);

	/**addToTree
	* @param name Name of the node/feature to be added
	* @param method Name of the discretisation method to be added
	*
	* Adds discretisation information to the property tree
	*/
        void addToTree(const std::string& name, const std::string& method);

	/**addToTree
	* @param name Name of the node/feature to be added
	* @param method Name of the discretisation method to be added
	* @param value Parameter for the discretisation method
	*
	* Adds discretisation information to the property tree
	*/
        void addToTree(const std::string& name, const std::string& method, float value);


	private:

	/**convertMethodToIntegerCode
	*
	* @param method A discretisation method in name representation
	*
	* @return An integer representing a distinct discretisation method
	*
	* Convertes the full name representation of the discretisation methods to a integer encoding
	*/
	unsigned int convertMethodToIntegerCode(const std::string& method) const;

	/**initialiseNameVector
	*
	* Initialises a vector to convert discretisation methods from integer code to full name
	*/
	void initialiseNameVector();

	/**initialiseMethodMap
	*
	* Initialises an unordered_map to convert discretisation method full names to integer code
	*/
        void initialiseMethodMap();

	//Unorderd_map to map discretisation method names to a dense integer code
	std::unordered_map<std::string,int> methodToInt_;

	//Vector mapping integer code to method names
	std::vector<std::string> intToMethod_;

	//A boost property tree holding the json file
	boost::property_tree::ptree pt_;
	
};

#endif
