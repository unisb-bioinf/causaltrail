#include "networkvis.h"
#include "NodeGui.h"
#include "edge.h"
#include <QGridLayout>
#include <QWheelEvent>
#include <QKeyEvent>


NetworkVis::NetworkVis(QWidget *parent, NetworkController& nc)
    :QGraphicsView(parent)
{
    createLayout(parent);

    createScene();

    loadNoads(nc);

    loadEdges(nc);

    forceDirectedLayout();

}

void NetworkVis::createLayout(QWidget *parent){
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(this);
    parent->setLayout(layout);
    layout->setMargin(0);
    parent->update();
}

void NetworkVis::createScene(){
    scence_ = new QGraphicsScene(this);
    scence_->setItemIndexMethod(QGraphicsScene::NoIndex);
    this->setScene(scence_);
    setCacheMode(CacheBackground);
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
}

void NetworkVis::loadNoads(NetworkController& nc){
    pointerVec_.resize(nc.getNetwork().size());
    for (Node& node : nc.getNetwork().getNodes()){
        NodeGui* newNode = new NodeGui(node.getID(), node.getName());
        pointerVec_[node.getID()]=newNode;
        scence_->addItem(newNode);
    }
}

void NetworkVis::loadEdges(NetworkController& nc){
    for (auto& node : nc.getNetwork().getNodes()){
        NodeGui* tar = pointerVec_[node.getID()];
        for (auto& parent : node.getParents()){
            NodeGui* src = pointerVec_[parent];
            Edge *newEdge = new Edge(src,tar);
            scence_->addItem(newEdge);
            pointerVecEdges_.push_back(newEdge);
        }
    }
}

void NetworkVis::forceDirectedLayout(){
    int limit = 1000;
    for (auto* node : pointerVec_){
        int randX = -350+ (qrand() % (700));
        int randY = -350 + (qrand() % (700));
        node->setPos(randX,randY);
    }
    for (int i = 0; i<limit; i++){
        for (auto* node : pointerVec_){
            node->calculateForces();
        }
    }
    shiftNodes();
    centerOn(0,0);
}

void NetworkVis::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void NetworkVis::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, event->delta() / 240.0));
}

void NetworkVis::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void NetworkVis::zoomIn()
{
    scaleView(qreal(1.2));
}

void NetworkVis::zoomOut()
{
    scaleView(1 / qreal(1.2));
}

void NetworkVis::shiftNodes()
{
    QPointF uM = pointerVec_[0]->pos();
    QPointF lM = pointerVec_[0]->pos();
    for (auto* node : pointerVec_){
        if (uM.y() < node->pos().y()){
                uM = node->pos();
        }
        if (lM.y() > node->pos().y()){
                lM = node->pos();
        }
    }
    QPointF shift = -((uM+lM)/2);
    for (auto* node : pointerVec_){
        node->setPos(node->pos()+shift);
    }
}

const std::vector<NodeGui*>& NetworkVis::getNodeGuiVec()
{
    return pointerVec_;
}

const std::vector<Edge *> &NetworkVis::getEdgeVec()
{
    return pointerVecEdges_;
}

QSize NetworkVis::sizeHint() const
{
    return QSize(width_,1080);

}

void NetworkVis::setWidth(unsigned int width)
{
    width_=width;
}

void NetworkVis::removeNodeHighlighting(){
    for (NodeGui* node : pointerVec_){
        node->removeSelection();
    }
}

void NetworkVis::removeEdgeHighlighting(){
    for (Edge* edge : pointerVecEdges_){
        edge->removeHighlighting();
    }
}

void NetworkVis::doIntervention(unsigned int id, bool flag)
{
    for (NodeGui* node : pointerVec_){
        if (id == node->getID())
            node->doIntervention(flag);
    }
}

void NetworkVis::unDoDoInt(){
    for (NodeGui* node : pointerVec_){
        node->doIntervention(false);
    }
}

NodeGui* NetworkVis::getNode(unsigned int id){
    for (NodeGui* node : pointerVec_){
        if (id == node->getID()){
            return node;
        }
    }
    throw std::invalid_argument("Node not found");
}

void NetworkVis::addEdge(unsigned int id1, unsigned int id2)
{
    Edge* newEdge = new Edge(getNode(id1), getNode(id2));
    newEdge->setAdditionalEdge();
    additionalEdges_.push_back(newEdge);
    scence_->addItem(newEdge);
    newEdge->update();
}

void NetworkVis::removeEdge(unsigned int id1, unsigned int id2)
{
    for (Edge* edge : pointerVecEdges_){
        if (((edge->sourceNode()->getID()==id1) && (edge->destNode()->getID()==id2)) ||((edge->sourceNode()->getID()==id2) && (edge->destNode()->getID()==id1))){
            edge->setDeletedEdge(true);
            removedEdges_.push_back(edge);
        }
    }
}

void NetworkVis::restoreEdges()
{
     for (Edge* edge : removedEdges_){
        edge->setDeletedEdge(false);
     }
     removedEdges_.clear();
}

void NetworkVis::removeAdditionalEdges()
{
    for (Edge* edge : additionalEdges_){
        scene()->removeItem(edge);
    }
    additionalEdges_.clear();
}

void NetworkVis::restoreEdge(QString source, QString target)
{
    for (std::vector<Edge*>::iterator it = removedEdges_.begin(); it != removedEdges_.end(); ++it){
        if (((*it)->sourceNode()->getName()==source) && ((*it)->destNode()->getName()==target)){
            (*it)->setDeletedEdge(false);
        }
    }
}

void NetworkVis::removeAdditionalEdge(QString source, QString target)
{
    for (std::vector<Edge*>::iterator it = additionalEdges_.begin(); it != additionalEdges_.end(); ++it){
        if (((*it)->sourceNode()->getName()==source) && ((*it)->destNode()->getName()==target)){
            scene()->removeItem(*it);
            additionalEdges_.erase(it);
            break;
        }
    }
}

std::vector<std::pair<unsigned int, unsigned int> > NetworkVis::getAddedEdgeIDs()
{
    std::vector<std::pair<unsigned int, unsigned int>> edgeList;
    for (Edge* edge : additionalEdges_){
        edgeList.push_back(std::make_pair(edge->sourceNode()->getID(),edge->destNode()->getID()));
    }
    return edgeList;
}

std::vector<std::pair<unsigned int, unsigned int> > NetworkVis::getRemovedEdgeIDs()
{
    std::vector<std::pair<unsigned int, unsigned int>> edgeList;
    for (Edge* edge : removedEdges_){
        edgeList.push_back(std::make_pair(edge->sourceNode()->getID(),edge->destNode()->getID()));
    }
    return edgeList;
}

void NetworkVis::mousePressEvent(QMouseEvent *event)
{
    removeNodeHighlighting();
    removeEdgeHighlighting();
    QGraphicsView::mousePressEvent(event);
}

void NetworkVis::originalNodeState(){
    for (NodeGui* node : pointerVec_){
        node->originalState();
    }
}
