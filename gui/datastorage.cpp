#include "datastorage.h"

dataStorage::dataStorage()
{

}

void dataStorage::saveSession(std::vector<NetworkInstance> &currentNetworks, QString filename)
{
 std::ofstream output;
 output.open(filename.toStdString());
 for (NetworkInstance& networkInst : currentNetworks){
     output<<"network"<<std::endl;
     output<<networkInst.getNaOrTgf().toStdString()<<std::endl;
     output<<networkInst.getSif().toStdString()<<std::endl;
     output<<networkInst.getDataFile().toStdString()<<std::endl;
     output<<networkInst.getDiscretisationControlFile().toStdString()<<std::endl;
     for (unsigned int i = 0; i < networkInst.getDeselectedSamples().size(); i++){
	 	output<<networkInst.getDeselectedSamples()[i]<<"\t";
     }
	 output<<std::endl;
     for (unsigned int i = 0; i < networkInst.getNumberOfQueries(); i++){
        output<<"query"<<std::endl;
        output<<networkInst.getQuery(i).toStdString()<<std::endl;
        output<<"queryItems"<<std::endl;
        for (QString& queryItem : networkInst.getQueryItems(i)){
            output<<queryItem.toStdString()<<std::endl;
        }
        output<<"conditionItems"<<std::endl;
        for (QString& conditionItem : networkInst.getConditionItems(i)){
            output<<conditionItem.toStdString()<<std::endl;
        }
        output<<"interventionItems"<<std::endl;
        for (QString& interventionItem : networkInst.getInterventionItems(i)){
            output<<interventionItem.toStdString()<<std::endl;
        }
        output<<"edgeAddRemItems"<<std::endl;
        for (QString& edgeAddRemItem : networkInst.getEdgeAddRemItems(i)){
            output<<edgeAddRemItem.toStdString()<<std::endl;
        }
     }
 }
 output.close();
}

void dataStorage::createQueryBatchFile(NetworkInstance &currentNetwork, QString filename)
{
    std::ofstream output;
    output.open(filename.toStdString());
    for (unsigned int i = 0; i< currentNetwork.getNumberOfQueries(); i++){
        output<<currentNetwork.getQuery(i).toStdString()<<std::endl;
    }
    output.close();
}

const std::vector<unsigned int> fromString(const std::string& line){
	std::vector<unsigned int> temp;

	return temp;
}

void dataStorage::loadSession(QString filename)
{
    QString naOrTgf;
    QString sif;
    QString sample;
    QString controlSample;
    std::ifstream input;
	std::stringstream deletedSamplesString;
    std::string line;
	std::vector<unsigned int> deletedSamples;
    input.open(filename.toStdString());
    std::getline(input,line);
    while(!input.eof()){
         if (line == "network"){
            qmaVec_.push_back(QueryManager());
            input >> line;
            naOrTgf = QString::fromStdString(line);
            input >> line;
            sif = QString::fromStdString(line);
            input >> line;
            sample = QString::fromStdString(line);
            input >> line;
            controlSample = QString::fromStdString(line);
			input >> line;
			deletedSamplesString << line;
			int i;
			while (deletedSamplesString >> i)
				deletedSamples.push_back(i);
            std::getline(input,line);
            networkInformation_.push_back(std::make_tuple(naOrTgf,sif,sample,controlSample,deletedSamples));
            std::getline(input,line);
        }
        if (line == "query"){
            std::getline(input,line);
            while (! isTerminal(QString::fromStdString(line),input)){
                qmaVec_.back().storeQuery(QString::fromStdString(line));
                std::getline(input,line);
                }
       }
       if (line == "queryItems"){
           std::getline(input,line);
           while (! isTerminal(QString::fromStdString(line),input)){
               qmaVec_.back().storeQueryItem(QString::fromStdString(line));
               std::getline(input,line);
               }
        }
        if (line == "conditionItems"){
            std::getline(input,line);
            while (! isTerminal(QString::fromStdString(line),input)){
                qmaVec_.back().storeConditionItem(QString::fromStdString(line));
                std::getline(input,line);
            }
        }
        if (line == "interventionItems"){
            std::getline(input,line);
            while (! isTerminal(QString::fromStdString(line),input)){
                qmaVec_.back().storeInterventionItem(QString::fromStdString(line));
                std::getline(input,line);
            }
        }
       if (line == "edgeAddRemItems"){
            std::getline(input,line);
            while (! isTerminal(QString::fromStdString(line),input)){
                qmaVec_.back().storeAddRemEdgeItem(QString::fromStdString(line));
                std::getline(input,line);
            }
        }
    }
    input.close();
}

unsigned int dataStorage::getNumberOfLoadedNetworks(){
    return networkInformation_.size();
}

const QString& dataStorage::getNAorTGf(unsigned int index){
    return std::get<0>(networkInformation_[index]);
}

const QString& dataStorage::getSif(unsigned int index){
   return std::get<1>(networkInformation_[index]);
}

const QString& dataStorage::getData(unsigned int index){
   return std::get<2>(networkInformation_[index]);
}

const QString& dataStorage::getDiscretiseControl(unsigned int index){
    return std::get<3>(networkInformation_[index]);
}

const std::vector<unsigned int>& dataStorage::getDeSelectedData(unsigned int index){
    return std::get<4>(networkInformation_[index]);
}

bool dataStorage::isTerminal(QString line, std::ifstream& input)
{
    return ((input.eof()) || (line == "query") || (line =="queryItems") || (line =="conditionItems") || (line == "interventionItems") || (line == "edgeAddRemItems") || (line == "network"));
}

QueryManager& dataStorage::getQma(unsigned int index){
    return qmaVec_[index];
}
