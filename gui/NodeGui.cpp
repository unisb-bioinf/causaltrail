#include "NodeGui.h"
#include "edge.h"

#include <QtCore/QString>
#include <QtGui/QCursor>
#include <QFontMetrics>
#include <QtWidgets/QGraphicsScene>

const qreal ADJUST = 2;

NodeGui::NodeGui(unsigned int id, std::string name)
    : QGraphicsEllipseItem(-35 - ADJUST, -10 - ADJUST, 70 + ADJUST, 20 + ADJUST),
      id_(id),
      name_(name),
      abbrev_(QString::fromStdString(name)),
      nodeColor(Qt::gray),
      secondNodeColor(Qt::gray),
      colorFixed(false)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setCursor(Qt::ArrowCursor);
    setAcceptHoverEvents(true);
    setZValue(-1);

    if (name.size() > 12){
        setToolTip(abbrev_);
    }

    if (abbrev_.size() > 12){
        abbrev_.resize(12);
        for (int i = 10; i<12; i++){
            abbrev_[i]='.';
        }
    }

	QGraphicsSimpleTextItem* child = new QGraphicsSimpleTextItem(this);
	QFont f("Monospace", 6);
	child->setFont(f);
	child->setText(abbrev_);
	child->setPos(getStart(abbrev_), - QFontMetrics(f).height() / 2.f);

	setColors(nodeColor, secondNodeColor);
}

void NodeGui::addEdge(Edge *edge){
    edges_.push_back(edge);
    edge->adjust();
}

int NodeGui::getStart(const QString& name){
    int size = name.size();
    if (size > 12){
        return -30;
    }
    if (size % 2 == 0){
        return -(ceil((size+1)/2)*5);
        }
    else{
    return -(ceil((size)/2)*5)-3;
    }
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
    return QGraphicsEllipseItem::itemChange(change, value);
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

void NodeGui::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    removeHighlighting();
    QGraphicsEllipseItem::mouseDoubleClickEvent(event);
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

    setColors(QColor::fromHsv(210, 220, 250));
    colorFixed=true;
    if (originalColor != Qt::gray){
        setColors(originalColor, originalColor2);
    }

    QGraphicsEllipseItem::contextMenuEvent(event);
}

void NodeGui::hoverEnterEvent(QGraphicsSceneHoverEvent*)
{
    if (!colorFixed){
        setColors(QColor::fromHsv(210, 100, 250));
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
		setColors(Qt::gray);
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
        setColors(color);
    }
    else{
        setSecondaryColor(color);
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
            setColors(Qt::gray);
        }
        else{
            setColors(secondNodeColor);
        }
    }
    else{
        setColors(nodeColor);
    }
    update();
}

void NodeGui::originalState(){
	setColors(Qt::gray);
    colorFixed=false;
    update();
}

void NodeGui::setColors(const QColor& primary, const QColor& secondary)
{
	nodeColor = primary;
	secondNodeColor = secondary;

    QLinearGradient g(rect().x(), 0, rect().x() + rect().width(), 0);

    g.setColorAt(0, primary);
    g.setColorAt(1, secondary);

    g.setStops({
		QGradientStop(0, nodeColor),
		QGradientStop(0.5, nodeColor),
		QGradientStop(0.5001, secondNodeColor),
		QGradientStop(1.0, secondNodeColor)
	});

	setBrush(g);
}

void NodeGui::setColors(const QColor& color)
{
	nodeColor = color;
	secondNodeColor = color;

	setBrush(color);
}

void NodeGui::setPrimaryColor(const QColor& primary)
{
	setColors(primary, secondNodeColor);
}

void NodeGui::setSecondaryColor(const QColor& secondary)
{
	setColors(nodeColor, secondary);
}
