#include "networkvis.h"
#include "NodeGui.h"
#include "edge.h"
#include "../core/DotReader.h"
#include "../core/Network.h"

#include <QtCore/QProcess>
#include <QtGui/QWheelEvent>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QGridLayout>
#include <QtSvg/QSvgGenerator>

NetworkVis::NetworkVis(QWidget *parent, const Network& net)
    : QGraphicsView(parent),
      net_(net)
{
    createLayout(parent);

    createScene();

    loadNodes();

    loadEdges();

    layoutGraph();
}

void NetworkVis::createLayout(QWidget *parent){
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(this);
    parent->setLayout(layout);
    layout->setMargin(0);
    parent->update();
}

void NetworkVis::createScene(){
    QGraphicsScene* scence = new QGraphicsScene(this);
    scence->setItemIndexMethod(QGraphicsScene::NoIndex);
    this->setScene(scence);
    setCacheMode(CacheBackground);
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
}

void NetworkVis::loadNodes() {
    pointerVec_.resize(net_.size());
    for (const Node& node : net_.getNodes()) {
        NodeGui* newNode = new NodeGui(node.getID(), node.getName());
        pointerVec_[node.getID()]=newNode;
        scene()->addItem(newNode);
    }
}

void NetworkVis::loadEdges() {
    for (const auto& node : net_.getNodes()) {
        NodeGui* tar = pointerVec_[node.getID()];
        for (auto& parent : node.getParents()){
            NodeGui* src = pointerVec_[parent];
            Edge *newEdge = new Edge(src,tar);
            scene()->addItem(newEdge);
            pointerVecEdges_.push_back(newEdge);
        }
    }
}

void NetworkVis::layoutGraph() {
	if(!dotLayout()) {
		forceDirectedLayout();
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

bool NetworkVis::dotLayout() {
	QProcess dotProcess(this);

	dotProcess.start("dot");
	writeDot_(dotProcess, net_);
	dotProcess.closeWriteChannel();
	dotProcess.waitForFinished();

	if(dotProcess.error() != QProcess::UnknownError) {
		return false;
	}

	return readDot_(dotProcess.readAllStandardOutput());
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
    scene()->addItem(newEdge);
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

void NetworkVis::contextMenuEvent(QContextMenuEvent* event)
{
	QGraphicsItem* item = itemAt(event->pos());

	if(!item) return;

	while(item->parentItem()) {
		item = item->parentItem();
	}

	switch(item->type()) {
		case QGraphicsEllipseItem::Type:
			emit context(static_cast<NodeGui*>(item), event);
			break;
		case Edge::Type:
			emit context(static_cast<Edge*>(item), event);
			break;
	}
}

void NetworkVis::mouseDoubleClickEvent(QMouseEvent* event)
{
	QGraphicsItem* item = itemAt(event->pos());

	if(!item) return;

	while(item->parentItem()) {
		item = item->parentItem();
	}

	if(item && item->type() == QGraphicsEllipseItem::Type) {
		emit doubleClick(static_cast<NodeGui*>(item));
	}
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

void NetworkVis::exportSVG(const QString& filename) {
	QSvgGenerator generator;
	QFile output(filename);
	generator.setOutputDevice(&output);
	generator.setSize(QSize(width(), height()));
	QPainter painter;
	painter.begin(&generator);
	render(&painter);
	painter.end();
	output.close();
}

bool NetworkVis::readDot_(const QByteArray& data)
{
	Dot::Reader parser;
	parser.parse(data.constData(), data.constData() + data.size());

	for(const auto& n : parser.getGraph().nodes) {
		size_t index;
		try {
			index = std::stoi(n.name.name);
		} catch(std::exception& e) {
			return false;
		}

		auto res = n.attributes.find("pos");

		if(res == n.attributes.end()) {
			return false;
		}

		auto list = QString::fromStdString(res->second).split(",");

		if(list.size() != 2) {
			return false;
		}

		pointerVec_[index]->setPos(QPointF(
			 list[0].toDouble(),
			-list[1].toDouble()
		));
	}

	return true;
}

void NetworkVis::writeDot_(QIODevice& dev, const Network& network) const
{
	dev.write("digraph G {\n");
	for(const Node& n : network.getNodes()) {
		for(auto& parent : n.getParents()) {
			dev.write(QByteArray::number(parent));
			dev.write(" -> ");
			dev.write(QByteArray::number(n.getID()));
			dev.write(";\n");
		}
	}
	dev.write("}\n");
}
