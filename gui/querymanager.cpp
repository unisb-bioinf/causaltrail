#include "querymanager.h"
#include "iostream"
QueryManager::QueryManager()
    :index_(0)
{
    queryItems_.resize(0);
    conditionItems_.resize(0);
    interventionItems_.resize(0);
    edgeAddRemItems_.resize(0);
}

bool QueryManager::isBeginning() const{
    return (index_==0);
}

bool QueryManager::isFinal() const{
    if (queryItems_.empty()){
        return true;
    }
    else{
          return (index_>=queryItems_.size()-1);
    }
}

QString& QueryManager::getPreviousQuery(){
    index_-=1;
    return queryVector_[index_];
}

QString& QueryManager::getSubsequentQuery(){
    index_+=1;
    return queryVector_[index_];
}

std::vector<QString>& QueryManager::getPreviousQueryItems(){
    return queryItems_[index_];
}

std::vector<QString>& QueryManager::getSubsequentQueryItems(){
    return queryItems_[index_];
}

std::vector<QString>& QueryManager::getPreviousConditionItems(){
    return conditionItems_[index_];
}

std::vector<QString>& QueryManager::getSubsequentConditionItems(){
    return conditionItems_[index_];
}

std::vector<QString>& QueryManager::getPreviousInterventionItems(){
    return interventionItems_[index_];
}

std::vector<QString>& QueryManager::getSubsequentInterventionItems(){
    return interventionItems_[index_];
}

std::vector<QString>& QueryManager::getPreviousEdgeChangeItems(){
    return  edgeAddRemItems_[index_];
}

std::vector<QString>& QueryManager::getSubsequentEdgeChangeItems(){
    return  edgeAddRemItems_[index_];
}

unsigned int QueryManager::getNumberOfQueries(){
    return queryVector_.size();
}

QString& QueryManager::getQuery(unsigned int index)
{
    return queryVector_[index];
}

std::vector<QString>& QueryManager::getQueryItems(unsigned int index){
    return queryItems_[index];
}

std::vector<QString>& QueryManager::getConditionItems(unsigned int index){
    return conditionItems_[index];
}

std::vector<QString>& QueryManager::getInterventionItems(unsigned int index){
    return interventionItems_[index];
}

std::vector<QString>& QueryManager::getEdgeAddRemItems(unsigned int index){
    return edgeAddRemItems_[index];
}

void QueryManager::storeQuery(QString& query, std::vector<QString> queries, std::vector<QString> conditions, std::vector<QString> interventions, std::vector<QString> edgeChanges){
    queryVector_.push_back(query);
    queryItems_.push_back(queries);
    conditionItems_.push_back(conditions);
    interventionItems_.push_back(interventions);
    edgeAddRemItems_.push_back(edgeChanges);
    index_=queryVector_.size()-1;
}

void QueryManager::storeQuery(QString line){
    queryVector_.push_back(line);
    queryItems_.push_back({});
    conditionItems_.push_back({});
    interventionItems_.push_back({});
    edgeAddRemItems_.push_back({});
    index_=queryVector_.size();
}

void QueryManager::storeQueryItem(QString line){
    queryItems_.back().push_back(line);
}

void QueryManager::storeConditionItem(QString line){
    conditionItems_.back().push_back(line);
}

void QueryManager::storeInterventionItem(QString line){
    interventionItems_.back().push_back(line);
}

void QueryManager::storeAddRemEdgeItem(QString line){
    edgeAddRemItems_.back().push_back(line);
}
