#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include "NetworkInstance.h"
#include "querymanager.h"
#include <tuple>
#include <vector>

class dataStorage
{
public:
    /**
     * @brief dataStorage Constructor
     * dataStorage handels session saving and loading as well as generation of batch files
     */
    dataStorage();

    /**
     * @brief saveSession
     * saves the current Session in a txt file
     * @param currentNetworks Vector containg the networkInstances of the current session
     * @param filename Name of the file to save the session to
     */
    void saveSession(std::vector<NetworkInstance>& currentNetworks, QString filename);

    /**
     * @brief createQueryBatchFile
     * Generates a file containing the current queries to use them later in batch processing
     * @param currentNetwork The underlying network
     * @param filename Name of the file to write the queries to
     */
    void createQueryBatchFile(NetworkInstance& currentNetwork, QString filename);

    /**
     * @brief loadSession
     * loads a previously saved session
     * @param filename name of the session file
     */
    void loadSession(QString filename);

    /**
     * @brief getNumberOfLoadedNetworks
     * @return the number of loaded networks
     */
    unsigned int getNumberOfLoadedNetworks();

    /**
     * @brief getNAorTGf
     * @param index Index of the current session
     * @return the .NA or .TGF file name included in session with the given index
     */
    const QString& getNAorTGf(unsigned int index);

    /**
     * @brief getSif
     * @param index Index of the current session
     * @return the .SIF file name included in session with the given index
     */
    const QString& getSif(unsigned int index);

    /**
     * @brief getData
     * @param index Index of the current session
     * @return the name of the datafile include in session with the given index
     */
    const QString& getData(unsigned int index);

    /**
     * @brief getDiscretiseControl
     * @param index Index of the current session
     * @return the name of the file containing discretisation control informaiton included in the sessino with the given index
     */
    const QString& getDiscretiseControl(unsigned int index);

    /**
     * @brief getQma
     * @param index Index of the current session
     * @return the QueryManager object of the NetworkInstance with the given index
     */
    QueryManager& getQma(unsigned int index);

private:
    /**
     * @brief isTerminal
     * @param line A distinct line of the session file
     * @param input InputFile Stream
     * @return true if the current line starts a new block in the structure of the session file
     */
    bool isTerminal(QString line, std::ifstream &input);

    /**
     * @brief networkInformation_
     * vector of tuples containing the filenames of NA/TGF, SIF, Data and Discretisation Control File for each NetworkInstance in the current session
     */
    std::vector<std::tuple<QString,QString,QString,QString>> networkInformation_;

    /**
     * @brief qmaVec_
     * Vector containing the QueryManagerObjects for each NetworkInstance in the current session
     */
    std::vector<QueryManager> qmaVec_;
};

#endif // DATASTORAGE_H
