#include "datastorage.h"

#include "DiscretisationSelection.h"
#include "NetworkInstance.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

void dataStorage::saveSession(const std::vector<NetworkInstance*>& currentNetworks, const QString& filename)
{
	unsigned int i = 1;
	pt::ptree output;
	for (NetworkInstance* networkInst : currentNetworks) {
		pt::ptree& networkOutput = output.add_child("network" + std::to_string(i), pt::ptree());
		storeNetwork_(networkOutput, networkInst);
		++i;
	}

	pt::write_json(filename.toStdString(), output);
}

void dataStorage::storeNetwork_(pt::ptree& output, const NetworkInstance* networkInst) const
{
	output.put("nodes", networkInst->getNaOrTgf().toStdString());
	if(!networkInst->getSif().isEmpty()) {
		output.put("topology", networkInst->getSif().toStdString());
	}
	output.put("data", networkInst->getDataFile().toStdString());
	output.add_child("discretisation", networkInst->getDiscretisationSettings().getPropertyTree());

	storeDeselectedSamples_(output, networkInst->getDeselectedSamples());
	storeQueries_(output, networkInst->getQMA().getQueries());
}

void dataStorage::storeDeselectedSamples_(
    pt::ptree& networkOutput,
    const std::vector<unsigned int>& samples) const
{
	if(samples.empty()) {
		return;
	}

	std::string tmp;
	for(unsigned int i : samples) {
		tmp += std::to_string(i) + ",";
	}
	tmp.resize(tmp.size() - 1);

	networkOutput.put("deselectedSamples", tmp);
}

void dataStorage::storeQueries_(pt::ptree& networkOutput,
                                const std::vector<QString>& queries) const
{
	if(queries.empty()) {
		return;
	}

	std::string tmp;
	for(const auto& query : queries) {
		tmp += query.toStdString() + ";";
	}
	tmp.resize(tmp.size() - 1);

	networkOutput.put("queries", tmp);
}

void dataStorage::createQueryBatchFile(NetworkInstance &currentNetwork, const QString& filename)
{
    std::ofstream output;
    output.open(filename.toStdString());
    for (unsigned int i = 0; i< currentNetwork.getQMA().getNumberOfQueries(); i++){
        output<<currentNetwork.getQMA().getQuery(i).toStdString()<<std::endl;
    }
    output.close();
}

std::vector<NetworkInstance*> dataStorage::loadSession(const QString& filename) const
{
	pt::ptree input;
	pt::read_json(filename.toStdString(), input);

	std::vector<NetworkInstance*> results;

	for(const auto& networks : input) {
		results.push_back(loadNetwork_(networks.second));
	}

	return results;
}

NetworkInstance*
dataStorage::loadNetwork_(const boost::property_tree::ptree& networkOutput) const
{
	NetworkInstance* instance = new NetworkInstance;

	instance->setNaOrTgf(QString::fromStdString(networkOutput.get<std::string>("nodes")));
	instance->setSif(QString::fromStdString(networkOutput.get("topology", "")));

	QString dataFile = QString::fromStdString(networkOutput.get("data", ""));

	// When there is no data it is not necessary/sensible
	// to look at any of the other fields.
	if(dataFile != "") {
		instance->setDataFile(dataFile);

		DiscretisationSettings settings(networkOutput.get_child("discretisation"));
		instance->setDiscretisationSettings(settings);
		instance->setDeselectedSamples(loadDeselectedSamples_(networkOutput));
		loadQueries_(networkOutput, instance);
	}

	return instance;
}

std::vector<unsigned int> dataStorage::loadDeselectedSamples_(
    const pt::ptree& input) const
{
	using namespace boost::algorithm;
	std::vector<unsigned int> samples;

	std::string tmp = input.get("deselectedSamples", "");
	auto begin = make_split_iterator(tmp, first_finder(";", boost::is_equal()));
	for(; begin != split_iterator<std::string::iterator>(); ++begin) {
		samples.push_back(std::stoi(boost::copy_range<std::string>(*begin)));
	}

	return samples;
}

void
dataStorage::loadQueries_(const pt::ptree& input, NetworkInstance* net) const
{
	using namespace boost::algorithm;

	std::string tmp = input.get("queries", "");
	auto begin = make_split_iterator(tmp, first_finder(";", boost::is_equal()));
	for(; begin != split_iterator<std::string::iterator>(); ++begin) {
		net->getQMA().storeQuery(QString::fromStdString(boost::copy_range<std::string>(*begin)));
	}
}
