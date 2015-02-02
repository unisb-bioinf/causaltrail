#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QMessageBox>
#include <QMainWindow>

#include "edge.h"
#include "NodeGui.h"
#include "iostream"


NodeGui::NodeGui(unsigned int id, std::string name)
    :id_(id),name_(name),nodeColor(Qt::gray),secondNodeColor(Qt::gray),colorFixed(false)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setCursor(Qt::ArrowCursor);
    setAcceptHoverEvents(true);
    setZValue(-1);

    if (name.size() > 12){
        setToolTip(QString::fromStdString(name));
    }
    abbrev_ = name;
    if (abbrev_.size() > 12){
        abbrev_.resize(12);
        for (int i = 10; i<12; i++){
            abbrev_[i]='.';
        }
    }
}

void NodeGui::addEdge(Edge *edge){
    edges_.push_back(edge);
    edge->adjust();
}

QRectF NodeGui::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -35 - adjust, -10 - adjust,
                  70 + adjust, 20 + adjust);
}

QPainterPath NodeGui::shape() const
{
    QPainterPath path;
    path.addEllipse(-35, -10, 70, 20);
    return path;
}


int NodeGui::getStart(const std::string& name){
    int size = name.size();
    if (size > 12){
        return -30;
    }
    if (size % 2 == 0){
        return -(ceil((size+1)/2)*5);
        }
    else{
    return -(ceil((size)/2)*5)-1;
    }
}

void NodeGui::paint(QPainter *painter, const QStyleOptionGraphicsItem * , QWidget *)
{
    QLinearGradient g(-35,0,35,0);
    g.setColorAt(0,nodeColor);
    g.setColorAt(1,secondNodeColor);
    g.setStops({QGradientStop(0,nodeColor),QGradientStop(0.5,nodeColor),QGradientStop(0.5001,secondNodeColor),QGradientStop(1.0,secondNodeColor)});
    painter->setPen(Qt::NoPen);
    painter->setBrush(g);
    painter->drawEllipse(-35, -10, 70, 20);

    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-35, -10, 70, 20);

    painter->setFont(QFont("Monospace",7));
    painter->drawText(getStart(abbrev_),3,QString::fromStdString(abbrev_));
}

void NodeGui::calculateForces()
{
    if (!scene() || scene()->mouseGrabberItem() == this) {
        newPos = pos();
        return;
    }

    qreal xvel = 0;
    qreal yvel = 0;
    foreach (QGraphicsItem *item, scene()->items()) {
        NodeGui* node = dynamic_cast<NodeGui*> (item);
        if (!node)
            continue;

        QPointF vec = mapToItem(node, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        double l = 2.0 * (dx * dx + dy * dy);
        if (l > 0) {
            xvel += (dx * 200.0) / l;
            yvel += (dy * 200.0) / l;
        }
    }

    double weight = (edges_.size() + 1) * 15;
    for (auto* edge: edges_) {
        QPointF vec;
        if (edge->sourceNode() == this)
            vec = mapToItem(edge->destNode(), 0, 0);
        else
            vec = mapToItem(edge->sourceNode(), 0, 0);
        xvel -= vec.x() / weight;
        yvel -= vec.y() / weight;
    }
    if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1){
        xvel = 0;
        yvel = 0;
    }
    newPos = pos() + QPointF(xvel, yvel);
    setPos(newPos);
}

QVariant NodeGui::itemChange(GraphicsItemChange change, const QVariant &value){
    switch (change) {
        case ItemPositionHasChanged:
            for( auto* edge : edges_){
                edge->adjust();
            }
            break;
        default:
            break;
        };
    return QGraphicsItem::itemChange(change, value);
}

void NodeGui::resetAllNodesToNormalColor()
{
    foreach (QGraphicsItem *item, scene()->items()) {
        NodeGui* node = dynamic_cast<NodeGui*> (item);
        if (!node)
            continue;
        node->removeHighlighting();
    }
}

void NodeGui::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* ){
    removeHighlighting();
    emit doubleClick(id_,QString::fromStdString(name_));

}

void NodeGui::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    resetAllNodesToNormalColor();
    QColor originalColor = Qt::gray;
    QColor originalColor2 = Qt::gray;
    if (colorFixed){
        originalColor = nodeColor;
        originalColor2 = secondNodeColor;
    }
    nodeColor.setHsv(210,220,250);
    secondNodeColor.setHsv(210,220,250);
    colorFixed=true;
    if (originalColor != Qt::gray){
        nodeColor=originalColor;
        secondNodeColor=originalColor2;
    }
    update();
    emit context(QString::fromStdString(name_),id_,event);
    update();
}

void NodeGui::hoverEnterEvent(QGraphicsSceneHoverEvent*)
{
    if (!colorFixed){
        nodeColor.setHsv(210,100,250);
        secondNodeColor.setHsv(210,100,250);
        update();
    }
}

void NodeGui::hoverLeaveEvent(QGraphicsSceneHoverEvent*)
{
    if (!colorFixed){
        removeHighlighting();
    }
}


void NodeGui::removeHighlighting(){
    if (!colorFixed){
        nodeColor=Qt::gray;
        secondNodeColor=Qt::gray;
        update();
    }
}

void NodeGui::doIntervention(bool flag){
    QColor doColor;
    doColor.setHsv(60,30,250);
    for (auto* edge: edges_) {
        if (edge->destNode() == this){
            edge->setDeletedEdge(flag);
            edge->adjust();
        }
        if(flag){

            setColor(doColor);
        }
        else{
            removeColor(doColor);
        }
        update();
    }
}

unsigned int NodeGui::getID() const
{
    return id_;
}

QString NodeGui::getName()
{
    return QString::fromStdString(name_);
}

void NodeGui::setColorFixed(bool flag){
    colorFixed = flag;
}

void NodeGui::removeSelection(){
    QColor selection;
    selection.setHsv(210,220,250);
    if (nodeColor == selection){
        nodeColor=Qt::gray;
        secondNodeColor=Qt::gray;
        colorFixed=false;
        update();
    }
}

void NodeGui::setColor(QColor color){
   QColor temp;
   temp.setHsv(210,220,250);
   if((nodeColor == temp) | (nodeColor == Qt::gray)){
        nodeColor=color;
        secondNodeColor=color;
    }
    else{
        secondNodeColor=color;
    }
    if (color != Qt::gray){
        colorFixed=true;
    }
    else
    {
        colorFixed=false;
    }
    update();
}

void NodeGui::removeColor(QColor color){
    if (nodeColor == color){
        if (secondNodeColor == color){
            nodeColor=Qt::gray;
            secondNodeColor=Qt::gray;
        }
        else{
            nodeColor=secondNodeColor;
        }
    }
    else{
        secondNodeColor=nodeColor;
    }
    update();
}

void NodeGui::originalState(){
    nodeColor=Qt::gray;
    secondNodeColor=Qt::gray;
    colorFixed=false;
    update();
}
