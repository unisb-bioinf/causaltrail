#include "DiscretisationFactory.h"

#include "DiscretiseRoundingBased.h"
#include "DiscretiseBracketMedians.h"
#include "DiscretisePT.h"
#include "DiscretiseThresholdBased.h"
#include "DiscretiseMapping.h"
#include "DiscretiseZScore.h"

#include "DiscretisationSettings.h"
#include "Discretisations.h"

#include <algorithm>
#include <locale>

DiscretisationFactory::DiscretisationFactory(
    const DiscretisationSettings& jsonTree)
    : jsonTree_(jsonTree)
{
	insert("ceil", [](const DiscretiserParameters&) {
		return std::make_unique<DiscretiseCeil>();
	});

	insert("floor", [](const DiscretiserParameters&) {
		return std::make_unique<DiscretiseFloor>();
	});

	insert("round", [](const DiscretiserParameters&) {
		return std::make_unique<DiscretiseRound>();
	});

	insert("arithmeticmean", [](const DiscretiserParameters&) {
		return std::make_unique<DiscretiseArithmeticMean>();
	});

	insert("harmonicmean", [](const DiscretiserParameters&) {
		return std::make_unique<DiscretiseHarmonicMean>();
	});

	insert("median", [](const DiscretiserParameters&) {
		return std::make_unique<DiscretiseMedian>();
	});

	insert("threshold", [](const DiscretiserParameters& params) {
		return std::make_unique<DiscretiseThreshold>(
		    params.getParameter<float>("threshold"));
	});

	insert("bracketmedians", [](const DiscretiserParameters& params) {
		return std::make_unique<DiscretiseBracketMedians>(
		    params.getParameter<unsigned int>("buckets"));
	});

	insert("pearsontukey", [](const DiscretiserParameters&) {
		return std::make_unique<DiscretisePT>();
	});

	insert("none", [](const DiscretiserParameters&) {
		return std::make_unique<DiscretiseMapping>();
	});

	insert("z-score", [](const DiscretiserParameters&) {
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

	std::locale loc;
	std::transform(method.begin(), method.end(), method.begin(),
	               [&loc](char c) { return std::tolower(c, loc); });

	auto it = generators_.find(method);

	if(it == generators_.end()) {
		throw std::invalid_argument("Unknown discretisation method '" + method +
		                            "'.");
	}

	return it->second->operator()(jsonTree_.getParameters(nodeName));
}
