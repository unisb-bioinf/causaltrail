#include "DiscretisationFactory.h"
#include "Discretisations.h"
#include "DiscretiseRoundingBased.h"
#include "DiscretiseBracketMedians.h"
#include "DiscretisePT.h"
#include "DiscretiseThresholdBased.h"
#include "DiscretiseMapping.h"
#include "DiscretiseZScore.h"

#include <algorithm>
#include <locale>

DiscretisationFactory::DiscretisationFactory(
    const SerializeDeserializeJson& jsonTree)
    : jsonTree_(jsonTree)
{
	insert("ceil", [](const std::string&, const SerializeDeserializeJson&) {
		return std::make_unique<DiscretiseCeil>();
	});

	insert("floor", [](const std::string&, const SerializeDeserializeJson&) {
		return std::make_unique<DiscretiseFloor>();
	});

	insert("round", [](const std::string&, const SerializeDeserializeJson&) {
		return std::make_unique<DiscretiseRound>();
	});

	insert("arithmeticmean",
	       [](const std::string&, const SerializeDeserializeJson&) {
		return std::make_unique<DiscretiseArithmeticMean>();
	});

	insert("harmonicmean",
	       [](const std::string&, const SerializeDeserializeJson&) {
		return std::make_unique<DiscretiseHarmonicMean>();
	});

	insert("median", [](const std::string&, const SerializeDeserializeJson&) {
		return std::make_unique<DiscretiseMedian>();
	});

	insert("threshold", [](const std::string& name,
	                       const SerializeDeserializeJson& properties) {
		return std::make_unique<DiscretiseThreshold>(
		    properties.getParameter<float>(name, "threshold"));
	});

	insert("bracketmedians", [](const std::string& name,
	                            const SerializeDeserializeJson& properties) {
		return std::make_unique<DiscretiseBracketMedians>(
		    properties.getParameter<unsigned int>(name, "buckets"));
	});

	insert("pearsontukey",
	       [](const std::string&, const SerializeDeserializeJson&) {
		return std::make_unique<DiscretisePT>();
	});

	insert("none", [](const std::string&, const SerializeDeserializeJson&) {
		return std::make_unique<DiscretiseMapping>();
	});

	insert("z-score", [](const std::string&, const SerializeDeserializeJson&) {
		return std::make_unique<DiscretiseZScore>();
	});
}

std::unique_ptr<Discretisations>
DiscretisationFactory::create(const std::string& nodeName)
{
	if(!jsonTree_.containsNode(nodeName)) {
		throw std::invalid_argument("Unknown node '" + nodeName + "'");
	}

	std::string method = jsonTree_.getMethod(nodeName);

	std::transform(method.begin(), method.end(), method.begin(),
	               [](char c) { return std::tolower(c); });

	auto it = generators_.find(method);

	if(it == generators_.end()) {
		throw std::invalid_argument("Unknown discretisation method '" + method +
		                            "'.");
	}

	return it->second->operator()(nodeName, jsonTree_);
}
