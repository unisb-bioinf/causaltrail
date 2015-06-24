#include "DiscretisationSettings.h"
#include "iostream"
#include <boost/property_tree/json_parser.hpp>

DiscretisationSettings::DiscretisationSettings() {}

DiscretisationSettings::DiscretisationSettings(const std::string& filename)
{
	importFile(filename);
}

DiscretisationSettings::~DiscretisationSettings() {}

void DiscretisationSettings::importFile(const std::string& filename)
{
	boost::property_tree::read_json(filename, pt_);
}

void DiscretisationSettings::exportToFile(const std::string& filename)
{
	boost::property_tree::write_json(filename, pt_);
}

bool DiscretisationSettings::containsNode(const std::string& nodeName) const
{
	const static std::string neg = "-1";
	return pt_.get<std::string>(nodeName, neg) != neg;
}

const std::string
DiscretisationSettings::getMethod(const std::string& nodeName) const
{
	return pt_.get<std::string>(nodeName + ".method");
}

void DiscretisationSettings::addToTree(const std::string& nodeName,
                                       const std::string& method)
{
	pt_.put(nodeName + ".method", method);
}

void DiscretisationSettings::addToTree(const std::string& nodeName,
                                       const std::string& method,
                                       const std::string& parameterID,
                                       const std::string& value)
{
	pt_.put(nodeName + ".method", method);
	pt_.put(nodeName + "." + parameterID, value);
}

DiscretiserParameters
DiscretisationSettings::getParameters(const std::string& nodeName) const
{
	return DiscretiserParameters(pt_.get_child(nodeName));
}

DiscretiserParameters::DiscretiserParameters(
    const boost::property_tree::ptree& parameters)
    : parameters_(parameters)
{
}
