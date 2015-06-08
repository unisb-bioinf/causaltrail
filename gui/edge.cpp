#define _USE_MATH_DEFINES
#include <cmath>
#undef _USE_MATH_DEFINES

#include "edge.h"
#include "NodeGui.h"

#include "QGraphicsLineItem"

Edge::Edge(NodeGui *src, NodeGui *tar)
    :srcN_(src),tarN_(tar), arrowSize_(5), fixedColor_(false), edgeColor_(Qt::black),lineStyle_(Qt::SolidLine)
{
    setCursor(Qt::ArrowCursor);
    setAcceptHoverEvents(true);
    src->addEdge(this);
    tar->addEdge(this);
    adjust();
}

NodeGui* Edge::sourceNode() const{
    return srcN_;
}

NodeGui* Edge::destNode() const{
    return tarN_;
}

void Edge::adjust()
{
    if (!srcN_ || !tarN_)
        return;

    QLineF line(mapFromItem(srcN_, 0, 0), mapFromItem(tarN_, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 37) / length, (line.dy() * 12) / length);
        srcP_ = line.p1() + edgeOffset;
        tarP_ = line.p2() - edgeOffset;
    } else {
        srcP_ = tarP_ = line.p1();
    }
}

QRectF Edge::boundingRect() const{
    if (!srcN_ || !tarN_)
            return QRectF();

        qreal penWidth = 1;
        qreal extra = (penWidth + arrowSize_) / 2.0;

        return QRectF(srcP_, QSizeF(tarP_.x() - srcP_.x(), tarP_.y() - srcP_.y())).normalized().adjusted(-extra,-extra,extra,extra);
}

QPainterPath Edge::shape() const
{
   QPainterPath path ;
   QPolygonF poly;
   poly.append(tarP_-(QPointF(2,2)));
   poly.append(tarP_+(QPointF(2,2)));
   poly.append(srcP_-(QPointF(2,2)));
   poly.append(srcP_+(QPointF(2,2)));
   path.addPolygon(poly);
   return path;
}

void Edge::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
    if (!srcN_ || !tarN_)
        return;

    QLineF line(srcP_, tarP_);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;


    painter->setPen(QPen(edgeColor_, 1, lineStyle_, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);


    painter->setPen(QPen(edgeColor_, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = 2*M_PI - angle;

    QPointF destArrowP1 = tarP_ + QPointF(sin(angle - M_PI / 3) * arrowSize_,
                                              cos(angle - M_PI / 3) * arrowSize_);
    QPointF destArrowP2 = tarP_ + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize_,
                                              cos(angle - M_PI + M_PI / 3) * arrowSize_);


    painter->setBrush(edgeColor_);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
}

void Edge::setDeletedEdge(bool flag){
    if (flag){
        edgeColor_=Qt::gray;
        lineStyle_=Qt::DotLine;
        setFixedEdge(true);
    }
    else{
        edgeColor_=Qt::black;
        lineStyle_=Qt::SolidLine;
        setFixedEdge(false);
    }
    update();
}

void Edge::setAdditionalEdge(){
    setColor(Qt::red);
    setFixedEdge(true);
}

void Edge::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    edgeColor_.setHsv(210,220,250);
    update();
    fixedColor_=true;
    emit context(this, event);
}

void Edge::hoverEnterEvent(QGraphicsSceneHoverEvent* )
{
    if (!fixedColor_){
        edgeColor_.setHsv(210,100,250);
        update();
    }
}

void Edge::hoverLeaveEvent(QGraphicsSceneHoverEvent* )
{
    if (!fixedColor_){
        edgeColor_=Qt::black;
        update();
    }
}

void Edge::setFixedEdge(bool flag)
{
    fixedColor_=flag;
}

void Edge::setColor(QColor color)
{
    if (!fixedColor_){
        edgeColor_=color;
    }
    update();
}

void Edge::removeHighlighting(){
    QColor temp;
    temp.setHsv(210,220,250);
    if (edgeColor_==temp){
        fixedColor_=false;
        edgeColor_=Qt::black;
        update();
    }
}
