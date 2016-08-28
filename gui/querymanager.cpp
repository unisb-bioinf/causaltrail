#include "querymanager.h"

#include <iostream>

QueryManager::QueryManager()
    :index_(0)
{
}

bool QueryManager::isBeginning() const{
    return (index_==0);
}

bool QueryManager::isFinal() const{
	return queryItems_.empty() || index_ + 1 >= queryItems_.size();
}

const QString& QueryManager::getPreviousQuery(){
    index_-=1;
    return queryVector_[index_];
}

const QString& QueryManager::getSubsequentQuery(){
    index_+=1;
    return queryVector_[index_];
}

unsigned int QueryManager::getNumberOfQueries() const {
    return queryVector_.size();
}

const std::vector<QString>& QueryManager::getQueries() const {
	return queryVector_;
}

const QString& QueryManager::getQuery(unsigned int index) const
{
    return queryVector_[index];
}

const std::vector<QString>& QueryManager::getQueryItems(unsigned int index) const {
    return queryItems_[index];
}

const std::vector<QString>& QueryManager::getConditionItems(unsigned int index) const {
    return conditionItems_[index];
}

const std::vector<QString>& QueryManager::getInterventionItems(unsigned int index) const {
    return interventionItems_[index];
}

const std::vector<QString>& QueryManager::getEdgeAddRemItems(unsigned int index) const {
    return edgeAddRemItems_[index];
}

unsigned int QueryManager::getCurrentQuery() const
{
	return index_;
}

void QueryManager::storeQuery(const QString& query, const std::vector<QString>& queries,
                              const std::vector<QString>& conditions, const std::vector<QString>& interventions,
                              const std::vector<QString>& edgeChanges)
{
    queryVector_.push_back(query);
    queryItems_.push_back(queries);
    conditionItems_.push_back(conditions);
    interventionItems_.push_back(interventions);
    edgeAddRemItems_.push_back(edgeChanges);
    index_=queryVector_.size()-1;
}

void QueryManager::storeQuery(const QString& line){
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
