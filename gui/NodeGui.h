#ifndef NODEGUI_H
#define NODEGUI_H

#include <QMainWindow>
#include <QGraphicsObject>
#include <QGraphicsView>

/**
 * This class is based on the Elastic Nodes Example from the Qt documentation:
 * http://doc.qt.io/qt-5/qtwidgets-graphicsview-elasticnodes-example.html#graphwidget-class-definition
 * (Accessed 31.01.2015)
 */

class Edge;

class QGraphicsSceneMouseEvent;

class NodeGui : public QGraphicsObject{
    Q_OBJECT

public:
    /**
     * @brief NodeGui
     * Constructor of class NodeGui
     * @param id Identiefer of the current node
     * @param name Name of the current node
     */
    NodeGui(unsigned int id, std::string name);

    enum { Type = UserType + 1 };

    int type() const Q_DECL_OVERRIDE { return Type; }

    /**
     * @brief addParent
     * Add a parent to the current node
     * @param parent Reference to a NodeGui object
     */
    void addParent(NodeGui& parent);

    /**
     * @brief addEdge
     * Adds an Edge to the the node
     * @param edge Pointer to an Edge object
     */
    void addEdge(Edge *edge);

    /**
     * @brief calculateForces
     * Calculating forces to layout the node in the entire network.
     * Considering repulsive and attractive forces
     */
    void calculateForces();

    /**
     * @brief boundingRect
     * Reimplementation of the boundingRect() method from QGraphicsObject
     * @return A QRectF object representing a rectangle. The node is positioned between its top left and bottom right boundary.
     */
    QRectF boundingRect() const;

    /**
     * @brief shape
     * Reimplementation of the shape() method from QGraphicsObject
     * @return An elliptic QPainterPath
     */
    QPainterPath shape() const;

    /**
     * @brief paint
     * Reimplementation of the paint() method from QGraphicsObject
     * @param painter A pointer to a QPainter object
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    /**
     * @brief removeHighlighting
     * Removes any highlighting effect from the node
     */
    void removeHighlighting();

    /**
     * @brief doIntervention
     * Changes the node appearnce depending on whether or not a do intervention is performed upon the node.
     * @param flag Bool indicating if a do intervention is performed on this node
     */
    void doIntervention(bool flag);

    /**
     * @brief getID
     * @return Identifier of this node
     */
    unsigned int getID() const;

    /**
     * @brief getName
     * @return Name of this node
     */
    QString getName();

    /**
     * @brief setColorFixed
     * Sets the color of the node to a changeable/unchangeable state
     * @param flag Bool indicating if the color of the node is fixed
     */
    void setColorFixed(bool flag);

    /**
     * @brief removeSelection
     * Removes the selection coloring from the node
     */
    void removeSelection();

    /**
     * @brief setColor
     * Sets the color of the node to the provided color
     * @param color new color of the node
     */
    void setColor(QColor color);

    /**
     * @brief removeColor
     * Removes the provided color. If a node has two different colors the method acts accordingly.
     * @param color Color to remove
     */
    void removeColor(QColor color);

    /**
     * @brief originalState
     * Restores the original state of the node
     */
    void originalState();

signals:

    /**
     * @brief doubleClick
     * Emits if a node has been double clicked
     * @param id Identifier of the current node
     * @param name Name of the current node
     */
    void doubleClick(unsigned int id, QString name);

    /**
     * @brief context
     * Emits if a node has been right clicked
     * @param name Name of the current node
     * @param id Identifier of the current node
     * @param event QGraphicsSceneContextMenuEvent pointer
     */
    void context(QString name, unsigned int id,QGraphicsSceneContextMenuEvent* event);

protected:

    /**
     * @brief itemChange
     * Depending on how the item changed, the method adapts the NodeGui object
     * @param change GraphicsItemChange object
     * @param value QVariant reference
     * @return QVariant Object
     */
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    /**
     * @brief mouseDoubleClickEvent
     * Handles the mouseDoubleClickEvent: Removes highlighting and emits the doubleClick event
     */
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*);

    /**
     * @brief contextMenuEvent
     * Handles the contextMenuEvent: Adapts the color of the node and emits the context event
     * @param event
     */
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

    /**
     * @brief hoverEnterEvent
     * Adapts the color of the node to represent the hoverEnter event
     */
    void hoverEnterEvent(QGraphicsSceneHoverEvent* );

    /**
     * @brief hoverLeaveEvent
     * Adapts the color of the node to represent the hoverLeave event
     */
    void hoverLeaveEvent(QGraphicsSceneHoverEvent*);

private:

    /**
     * @brief getStart
     * Calculates the start position for the name of the node in the visualisation such that it is positioned
     * in the middle of the node (if possible).
     * @param name Name of the Node
     * @return The start position of the node name in the visualisation of the node
     */
    int getStart(const std::string& name);

    /**
     * @brief resetAllNodesToNormalColor
     * Removes the highlting from all nodes in the current network
     */
    void resetAllNodesToNormalColor();

    /**
     * @brief id_
     * Identifier of the node
     */
    unsigned int id_;

    /**
     * @brief name_
     * Name of the nodeColor
     */
    std::string name_;

    /**
     * @brief abbrev_
     * Abbreviatet name of the node
     */
    std::string abbrev_;

    /**
     * @brief edges_
     * Vector holding pointers to all edges that are connected to the node
     */
    std::vector<Edge*> edges_;

    /**
     * @brief newPos
     * Calculated position of the node by the force directed layout
     */
    QPointF newPos;

    /**
     * @brief nodeColor
     * Color of the node
     */
    QColor nodeColor;

    /**
     * @brief secondNodeColor
     * Optional second color of a node
     */
    QColor secondNodeColor;

    /**
     * @brief colorFixed
     * Bool indicating if the color of the node is fixed
     */
    bool colorFixed;
};

#endif // NODEGUI_H
