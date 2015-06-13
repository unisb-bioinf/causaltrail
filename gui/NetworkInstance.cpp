#include "NetworkInstance.h"
#include "../core/Parser.h"
#include "NodeGui.h"
NetworkInstance::NetworkInstance()
    : trained_(false),
      argmax_(false),
      remainingNodesForEdgeAddition_(0),
      id1_(-1),
      id2_(-1),
      naOrTgf_("%"),
      sif_("%"),
      dataFile_("%"),
      discretisationControl_("%")
{
}

void NetworkInstance::loadNetwork(QString filename){
    nc_.loadNetwork(filename.toStdString());
}

void NetworkInstance::visualize(QWidget* tabwidget){
    nv_ = new NetworkVis(tabwidget, nc_.getNetwork());
}

void NetworkInstance::loadSamples(const QString& filename, const Discretiser::Discretisations& control){
    if (deselectedSamples_.empty()){
		nc_.loadObservations(filename.toStdString(), control);
	} else {
		nc_.loadObservations(filename.toStdString(), control,
		                     deselectedSamples_);
	}
    nc_.trainNetwork();
    trained_=true;
}

std::pair<float, std::vector<std::string>>
NetworkInstance::calculate(const std::string& query)
{
	return Parser(query, nc_).parseQuery().execute();
}

bool NetworkInstance::isFirstQuery(){
    return qma_.isBeginning();
}

bool NetworkInstance::isLastQuery(){
    return qma_.isFinal();
}

bool NetworkInstance::isTrained(){
    return trained_;
}

QString& NetworkInstance::getPreviousQuery(){
    return qma_.getPreviousQuery();
}

QString& NetworkInstance::getSubsequentQuery(){
    return qma_.getSubsequentQuery();
}

std::vector<QString> & NetworkInstance::getPreviousQueryItems(){
    std::vector<QString>& items = qma_.getPreviousQueryItems();
    visualizeNonInterventionNodes(items);
    return items;
}

std::vector<QString> & NetworkInstance::getSubsequentQueryItems(){
    std::vector<QString>& items = qma_.getSubsequentQueryItems();
    visualizeNonInterventionNodes(items);
    return items;
}

std::vector<QString> & NetworkInstance::getPreviousConditionItems(){
    std::vector<QString>& items = qma_.getPreviousConditionItems();
    visualizeCondition(items);
    return items;
}

std::vector<QString> & NetworkInstance::getSubsequentConditionItems(){
    std::vector<QString>& items = qma_.getSubsequentConditionItems();
    visualizeCondition(items);
    return items;
}

std::vector<QString> & NetworkInstance::getPreviousInterventionItems(){
    std::vector<QString>& items = qma_.getPreviousInterventionItems();
    visualizeInterventions(items);
    return items;
}

std::vector<QString> & NetworkInstance::getSubsequentInterventionItems(){
    std::vector<QString>& items = qma_.getSubsequentInterventionItems();
    visualizeInterventions(items);
    return items;
}

std::vector<QString> & NetworkInstance::getPreviousEdgeChangeItems(){
    std::vector<QString>& items = qma_.getPreviousEdgeChangeItems();
    visualizeEdgeChanges(items);
    return items;
}

std::vector<QString> & NetworkInstance::getSubsequentEdgeChangeItems(){
    std::vector<QString>& items = qma_.getSubsequentEdgeChangeItems();
    visualizeEdgeChanges(items);
    return items;
}

void NetworkInstance::storeQuery(QString query, std::vector<QString> queries, std::vector<QString> conditions, std::vector<QString> interventions, std::vector<QString> edgeChanges){
    qma_.storeQuery(query,queries,conditions,interventions,edgeChanges);
}

unsigned int NetworkInstance::getRowCountOfCurrentProbMatrix()
{
    return nc_.getNetwork().getNode(selectedNode_).getProbabilityMatrix().getRowCount();
}

unsigned int NetworkInstance::getColCountOfCurrentProbMatrix()
{
    return nc_.getNetwork().getNode(selectedNode_).getProbabilityMatrix().getColCount();
}

unsigned int NetworkInstance::getNumberOfParentsOfSelectedNode(){
    return nc_.getNetwork().getNode(selectedNode_).getNumberOfParents();
}

std::vector<std::string> NetworkInstance::getParentsOfSelectedNode(){
    auto parentList = nc_.getNetwork().getNode(selectedNode_).getParents();
    std::vector<std::string> temp;
    for (auto& parent : parentList){
        temp.push_back(nc_.getNetwork().getNode(parent).getName());
    }
    return temp;
}

float NetworkInstance::getProbabilityAt(unsigned int row, unsigned int col)
{
    return nc_.getNetwork().getNode(selectedNode_).getProbabilityMatrix()(col,row);
}

const std::vector<std::string> &NetworkInstance::getRowNamesOfProbabilityMatrix()
{
   return nc_.getNetwork().getNode(selectedNode_).getProbabilityMatrix().getRowNames();
}

const std::vector<std::string> &NetworkInstance::getColNamesOfProbabilityMatrix()
{
   return nc_.getNetwork().getNode(selectedNode_).getProbabilityMatrix().getColNames();
}

void NetworkInstance::visualizeNonInterventionNodes(std::vector<QString> &nonInterventions)
{
    for (QString& item : nonInterventions){
        QString name = item.split(" ")[0];
        if (name == "argmax"){
            name = item.split(" ")[1];
        }
        QColor queryColor;
        queryColor.setHsv(100,30,250);
        nv_->getNode(nc_.getNetwork().getNode(name.toStdString()).getID())->originalState();
        nv_->getNode(nc_.getNetwork().getNode(name.toStdString()).getID())->setColor(queryColor);
    }
}

void NetworkInstance::visualizeCondition(std::vector<QString> &condition)
{
    for (QString& item : condition){
        QString name = item.split(" ")[0];
        QColor queryColor;
        queryColor.setHsv(20,30,250);
        nv_->getNode(nc_.getNetwork().getNode(name.toStdString()).getID())->originalState();
        nv_->getNode(nc_.getNetwork().getNode(name.toStdString()).getID())->setColor(queryColor);
    }
}

void NetworkInstance::visualizeInterventions(std::vector<QString> &interventions)
{
    for (QString& item : interventions){
        QString name = item.split(" ")[0];
        nv_->getNode(nc_.getNetwork().getNode(name.toStdString()).getID())->originalState();
        setSelectedNode(nc_.getNetwork().getNode(name.toStdString()).getID(), name);
        doIntervention();
    }
}

void NetworkInstance::visualizeEdgeChanges(std::vector<QString> &edgeChanges)
{
   for (QString& item : edgeChanges){
        std::string id_ = item.split(" ")[0].toStdString();
        std::string name1 = item.split(" ")[1].toStdString();
        std::string name2 = item.split(" ")[2].toStdString();
        if (id_=="+"){
            nv_->addEdge(nc_.getNetwork().getNode(name1).getID(),nc_.getNetwork().getNode(name2).getID());
        }
        else{
            nv_->removeEdge(nc_.getNetwork().getNode(name1).getID(),nc_.getNetwork().getNode(name2).getID());
        }
    }
}

void NetworkInstance::layout(){
    nv_->layoutGraph();
}

const std::vector<std::string>& NetworkInstance::getValues(unsigned int id){
  return nc_.getNetwork().getNode(id).getValueNamesProb();
}

void NetworkInstance::setArgMax(bool flag){
    argmax_=flag;
}

bool NetworkInstance::isArgMax(){
    return argmax_;
}

void NetworkInstance::initialiseEdgeAddition(){
    remainingNodesForEdgeAddition_=2;
    id1_=-1;
    id2_=-1;
}

bool NetworkInstance::EdgeAddition(){
    return (remainingNodesForEdgeAddition_ > 0);
}

bool NetworkInstance::checkEdgeAddition(unsigned int id){
    //TODO Remove Edges to be removed
    std::vector<std::pair<unsigned int, unsigned int>> addedEdgeList = nv_->getAddedEdgeIDs();
    std::vector<std::pair<unsigned int, unsigned int>> removedEdgeList = nv_->getRemovedEdgeIDs();
    if (id1_ != -1){
        return nc_.isEdgePossible(id1_, id, addedEdgeList, removedEdgeList);
    }
    return false;
}

void NetworkInstance::NodeForEdgeAdditionSelected(unsigned int id){
    if (id1_==-1){
        id1_=id;
    }
    else{
        id2_=id;
        nv_->addEdge(id1_,id2_);
        id1_=-1;
        id2_=-1;
    }
    remainingNodesForEdgeAddition_--;
}

void NetworkInstance::EdgeRemoval(unsigned int srcId, unsigned int tarId) {
   id1_ = srcId;
   id2_ = tarId;
}

const std::string& NetworkInstance::removedEdgeSourceName() const {
    return nc_.getNetwork().getNode(id1_).getName();
}

const std::string& NetworkInstance::removedEdgeTargetName() const {
    return nc_.getNetwork().getNode(id2_).getName();
}

void NetworkInstance::RemoveSelectedEdge(){
    nv_->removeEdge(id1_,id2_);
    id1_=-1;
    id2_=-1;
}

const std::vector<NodeGui*>& NetworkInstance::getNodeGuiVec(){
    return nv_->getNodeGuiVec();
}

const std::vector<Edge*>& NetworkInstance::getEdgeVec(){
    return nv_->getEdgeVec();
}

void NetworkInstance::setSelectedNode(unsigned int id, QString name)
{
    selectedNode_=id;
    selectedNodeName_=name;
}

QString NetworkInstance::getSelectedNodeName(){
    return selectedNodeName_;
}

unsigned int NetworkInstance::getSelectedNodeID()
{
    return selectedNode_;
}

void NetworkInstance::doIntervention(){
    nv_->doIntervention(selectedNode_,true);
}

void NetworkInstance::removeDoIntervention(QString name){
    selectedNode_=nc_.getNetwork().getNode(name.toStdString()).getID();
    nv_->doIntervention(selectedNode_,false);
}

void NetworkInstance::removeHighlighting(){
    nv_->removeNodeHighlighting();
    nv_->removeEdgeHighlighting();
}

void NetworkInstance::restoreOriginalNetworkRepresentation(){
    nv_->unDoDoInt();
    nv_->originalNodeState();
    nv_->restoreEdges();
    nv_->removeAdditionalEdges();
}

void NetworkInstance::reverseEdgeRemoval(QString source, QString target){
    nv_->restoreEdge(source, target);
}

void NetworkInstance::reverseEdgeAddition(QString source, QString target){
    nv_->removeAdditionalEdge(source, target);
}

void NetworkInstance::colorNode(unsigned int id, QColor color){
    nv_->getNode(id)->setColor(color);
}

void NetworkInstance::removeNodeColor(QString name,QColor color){
    nv_->getNode(nc_.getNetwork().getNode(name.toStdString()).getID())->removeColor(color);
}

QString NetworkInstance::getNaOrTgf(){
    return naOrTgf_;
}

QString NetworkInstance::getSif(){
    return sif_;
}

QString NetworkInstance::getDataFile(){
    return dataFile_;
}

unsigned int NetworkInstance::getNumberOfQueries() const{
    return qma_.getNumberOfQueries();
}

const QString& NetworkInstance::getQuery(unsigned int index) const
{
    return qma_.getQuery(index);
}
std::vector<QString>& NetworkInstance::getQueryItems(unsigned int index){
    std::vector<QString>& items = qma_.getQueryItems(index);
    visualizeNonInterventionNodes(items);
    return items;
}

std::vector<QString>& NetworkInstance::getConditionItems(unsigned int index){
     std::vector<QString>& items = qma_.getConditionItems(index);
     visualizeCondition(items);
     return items;
}

std::vector<QString>& NetworkInstance::getInterventionItems(unsigned int index){
     std::vector<QString>& items = qma_.getInterventionItems(index);
     visualizeInterventions(items);
     return items;
}

std::vector<QString>& NetworkInstance::getEdgeAddRemItems(unsigned int index){
     std::vector<QString>& items = qma_.getEdgeAddRemItems(index);
     visualizeEdgeChanges(items);
     return items;
}

QString NetworkInstance::getDiscretisationControlFile(){
    return discretisationControl_;
}

void NetworkInstance::setNaOrTgf(const QString& filename){
    naOrTgf_= filename;
}

void NetworkInstance::setSif(const QString& filename){
    sif_= filename;
}

void NetworkInstance::setDataFile(const QString& filename){
    dataFile_= filename;
}

void NetworkInstance::setDiscretisationControlFile(const QString& filename){
    discretisationControl_ = filename;
}

void NetworkInstance::setQMA(QueryManager& qma){
    qma_=qma;
}

void NetworkInstance::resetNetwork(){
    nc_.getNetwork().reset();
}

void NetworkInstance::setDeselectedSamples(const std::vector<unsigned int> &ids)
{
    deselectedSamples_=ids;
}

std::vector<unsigned int> &NetworkInstance::getDeselectedSamples()
{
    return deselectedSamples_;
}
