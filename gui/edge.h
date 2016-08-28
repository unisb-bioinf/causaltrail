#ifndef EDGE_H
#define EDGE_H

#include <QtWidgets/QGraphicsObject>

/**
 * This class is based on the Elastic Nodes Example from the Qt documentation:
 * http://doc.qt.io/qt-5/qtwidgets-graphicsview-elasticnodes-example.html#graphwidget-class-definition
 * (Accessed 31.01.2015)
 */

class NodeGui;

class Edge : public QGraphicsItem {

public:
    /**
     * @brief Edge
     * Constructor of class Edge
     * @param src Pointer to the NodeGui object being the source of this edge
     * @param tar Pointer to the NodeGui object being the target of this edge
     */
    Edge(NodeGui *src, NodeGui *tar);

    enum { Type = UserType + 2 };
    int type() const Q_DECL_OVERRIDE { return Type; }

    /**
     * @brief adjust
     * Reimplementation of the adjust() method of QGraphicsObject
     */
    void adjust();

    /**
     * @brief boundingRect
     * Reimplementation of the boundingRect() method of QGraphicsObject
     * @return A QRectF() object. Within its top left and bottom right boundary, the edge is located.
     */
    QRectF boundingRect() const;

    /**
     * @brief shape
     * Reimplementation of the shape() method of QGraphicsObject
     * @return A QPainterPath, which in this case is a polygon that is 5 times as broad as the edge itself to allow distinct and precise selection of the edge using mouseOver/Hover events
     */
    QPainterPath shape() const;

    /**
     * @brief paint
     * Reimplementation of the paint() method of QGraphicsObject
     * @param painter A pointer to a QPainter object
     * @param option not used
     * @param widget not used
     */
    void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /**
     * @brief sourceNode
     * @return A pointer to the NodeGUI object that is the source of this edge
     */
    NodeGui* sourceNode() const;

    /**
     * @brief destNode
     * @return A pointer to the NodeGUI object that is the target of this edge
     */
    NodeGui* destNode() const;

    /**
     * @brief setDeletedEdge
     * Adapts the Color of this edge with respect to edge removals
     * @param flag bool indicating if this edge is deleted or not
     */
    void setDeletedEdge(bool flag);

    /**
     * @brief setAdditionalEdge
     * Adapts the color of this edge with respect to edge additions
     */
    void setAdditionalEdge();

    /**
     * @brief setFixedEdge
     * Renders the color of this edge unchangeable/changeable depending on the given flag
     * @param flag Indicaties if the color of this edge is fixed
     */
    void setFixedEdge(bool flag);

    /**
     * @brief setColor
     * Sets the color of this edge to the given color
     * @param color New color of the edge
     */
    void setColor(QColor color);

    /**
     * @brief removeHighlighting
     * Removes the highlighting from an edge
     */
    void removeHighlighting();

protected:
    /**
     * @brief contextMenuEvent
     * Emits the context signal and adapts the color of the edge to dark blue to represent the context event
     * @param event QGraphicsSceneContextMenuEvent pointer
     */
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

    /**
     * @brief hoverEnterEvent
     * Changes the color of the edge to bright blue to indicate the hoverEnter event
     */
    void hoverEnterEvent(QGraphicsSceneHoverEvent*);

    /**
     * @brief hoverLeaveEvent
     * Changes the color of the edge to the original gray to indicate the hoverLeave event
     */
    void hoverLeaveEvent(QGraphicsSceneHoverEvent*);

private:
    /**
     * @brief srcN_
     * Pointer to the NodeGui object representing the source of this edge
     */
    NodeGui *srcN_;

    /**
     * @brief tarN_
     * Pointer to the NodeGui object representing the target of this edge
     */
    NodeGui *tarN_;

    /**
     * @brief srcP_
     * Coordinates of the source of this edge
     */
    QPointF srcP_;

    /**
     * @brief tarP_
     * Coordinates of the target of this edge
     */
    QPointF tarP_;

    /**
     * @brief arrowSize_
     * Size of the arrow of the edge
     */
    float arrowSize_;

    /**
     * @brief fixedColor_
     * Bool indicating if the color of this edge can be changed
     */
    bool fixedColor_;

    /**
     * @brief edgeColor_
     * The color of the edge
     */
    QColor edgeColor_;

    /**
     * @brief lineStyle_
     * The style of the edge (dotted or solid)
     */
    Qt::PenStyle lineStyle_;
};

#endif // EDGE_H
