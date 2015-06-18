#ifndef DISCRETISATIONSETTINGS_H
#define DISCRETISATIONSETTINGS_H

#include <boost/property_tree/ptree.hpp>
#include <unordered_map>

class DiscretiserParameters
{
	public:
	explicit DiscretiserParameters(
	    const boost::property_tree::ptree& parameters);

	template <typename T> T getParameter(const std::string& paramName) const
	{
		return parameters_.get<T>(paramName);
	}

	private:
	const boost::property_tree::ptree& parameters_;
};

class DiscretisationSettings
{
	public:
	/**Constructor
	*
	* Constructs a DiscretisationSettings object used to import and export json
	*files
	* containing information on data discretisation
	*/
	DiscretisationSettings();

	/**Detailed Constructor
	*
	* @param filename name of the json file that should be loaded
	*
	* Constructs a SerailzeDeserializeJson object representing the specified
	*json file
	*/
	DiscretisationSettings(const std::string& filename);

	/**Deconstructor
	*
	*/
	~DiscretisationSettings();

	/**importFile
	*
	* @param filename Name of the file containing discretisation information
	*
	* Creates a boost property tree representing the specified json file
	*/
	void importFile(const std::string& filename);

	/**exportToFile
	*
	* @param filename Name of the json file to be created
	*
	* Exports the current boost property tree to a json file with the given
	*name.
	*/
	void exportToFile(const std::string& filename);

	/**containsNode
	*
	* @param name Name of the node/feature for which existence should be checked
	*
	* @return bool True if the property tree contains a entry for the specified
	*node
	*
	* Checks if the property tree contains an entry for a certain node
	*/
	bool containsNode(const std::string& nodeName) const;

	/**getMethod
	*
	* @param name Name of the node/feature for which discretisation information
	*should be extracted
	*
	* @return std::string Name of the discretisation method for this node
	*
	* Extractes discretisation information from the property tree for a given
	*node/feature.
	*/
	const std::string getMethod(const std::string& nodeName) const;

	/**getParameter
	*
	* @param name Name of the node/feature for which discretisation information
	*should be extracted
	* @param parameterID Name of the discretisation method used for the
	*specified feature
	*
	* @return <T> Parameter of the specified discretisation method.
	* The second element contains parameters for the method (not neccessary in
	*all cases)
	*
	* Extractes discretisation information from the property tree for a given
	*node/feature.
	*/
	template <typename T>
	const T getParameter(const std::string& nodeName,
	                     const std::string& parameterID) const;

	DiscretiserParameters getParameters(const std::string& nodeName) const;

	/**addToTree
	* @param name Name of the node/feature to be added
	* @param method Name of the discretisation method to be added
	*
	* Adds discretisation information to the property tree
	*/
	void addToTree(const std::string& nodeName, const std::string& method);

	/**addToTree
	* @param name Name of the node/feature to be added
	* @param method Name of the discretisation method to be added
	* @param parameterID Identifier of the parameter which should be added
	* @param value Parameter for the discretisation method
	*
	* Adds discretisation information to the property tree
	*/
	void addToTree(const std::string& nodeName, const std::string& method,
	               const std::string& parameterID, const std::string& value);

	private:
	// A boost property tree holding the json file
	boost::property_tree::ptree pt_;
};

template <typename T>
const T
DiscretisationSettings::getParameter(const std::string& nodeName,
                                     const std::string& parameterID) const
{
	return pt_.get<T>(nodeName + "." + parameterID);
}

#endif // DISCRETISATIONSETTINGS_H
