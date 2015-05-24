#ifndef JPORTER_H
#define JPORTER_H

#include <boost/property_tree/ptree.hpp>
#include <unordered_map>

class JPorter{
	public:

	JPorter();

	~JPorter();

	void importFile(const std::string& filename);

	void printContent() const;

	void exportToFile(const std::string& filename);

	bool containsNode(const std::string&) const;

	std::pair<unsigned int, float> getDiscretisationDetails(const std::string&) const;

	void addToTree(const std::string& name, unsigned int method);
	
	void addToTree(const std::string& name, unsigned int method, float value);

        void addToTree(const std::string& name, const std::string& method);

        void addToTree(const std::string& name, const std::string& method, float value);


	private:

	unsigned int convertMethodToIntegerCode(const std::string& method) const;

	void initialiseNameVector();

        void initialiseMethodMap();

	std::unordered_map<std::string,int> methodToInt_;

	std::vector<std::string> intToMethod_;

	boost::property_tree::ptree pt_;
	
};

#endif
