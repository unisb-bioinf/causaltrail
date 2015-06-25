#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <string>
#include <vector>

class NetworkInstance;
class QString;

namespace boost {
	namespace property_tree {
		template<class Key, class Data, class KeyCompare>
		class basic_ptree;
		using ptree = basic_ptree <std::string, std::string, std::less<std::string>>;
	}
}

class dataStorage
{
public:
    /**
     * @brief saveSession
     * saves the current Session in a txt file
     * @param currentNetworks Vector containg the networkInstances of the current session
     * @param filename Name of the file to save the session to
     */
    void saveSession(const std::vector<NetworkInstance*>& currentNetworks, const QString& filename);

    /**
     * @brief createQueryBatchFile
     * Generates a file containing the current queries to use them later in batch processing
     * @param currentNetwork The underlying network
     * @param filename Name of the file to write the queries to
     */
    void createQueryBatchFile(NetworkInstance& currentNetwork, const QString& filename);

    /**
     * @brief loadSession
     * loads a previously saved session
     * @param filename name of the session file
     */
    std::vector<NetworkInstance*> loadSession(const QString& filename) const;

private:
    void storeNetwork_ (boost::property_tree::ptree& networkOutput, const NetworkInstance* networkInst) const;
	void storeDeselectedSamples_(boost::property_tree::ptree& networkOutput, const std::vector<unsigned int>& samples) const;
	void storeQueries_(boost::property_tree::ptree& networkOutput, const std::vector<QString>& queries) const;

    NetworkInstance* loadNetwork_(const boost::property_tree::ptree& networkOutput) const;
	std::vector<unsigned int> loadDeselectedSamples_(const boost::property_tree::ptree& input) const;
	void loadQueries_(const boost::property_tree::ptree& input, NetworkInstance* net) const;
};

#endif // DATASTORAGE_H
