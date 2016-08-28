#ifndef NODEGUI_H
#define NODEGUI_H

#include <QtWidgets/QGraphicsEllipseItem>

/**
 * This class is based on the Elastic Nodes Example from the Qt documentation:
 * http://doc.qt.io/qt-5/qtwidgets-graphicsview-elasticnodes-example.html#graphwidget-class-definition
 * (Accessed 31.01.2015)
 */

class Edge;

class QGraphicsSceneMouseEvent;

class NodeGui : public QGraphicsEllipseItem {
public:
    /**
     * @brief NodeGui
     * Constructor of class NodeGui
     * @param id Identiefer of the current node
     * @param name Name of the current node
     */
    NodeGui(unsigned int id, std::string name);

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

protected:

    /**
     * @brief itemChange
     * Depending on how the item changed, the method adapts the NodeGui object
     * @param change GraphicsItemChange object
     * @param value QVariant reference
     * @return QVariant Object
     */
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    /**
     * @brief mouseDoubleClickEvent
     * Handles the mouseDoubleClickEvent: Removes highlighting and emits the doubleClick event
     */
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*) override;

    /**
     * Handles the contextMenuEvent: Adapts the color of the node and emits the context event
	 *
     * @param event The triggered event.
     */
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

    /**
     * @brief hoverEnterEvent
     * Adapts the color of the node to represent the hoverEnter event
     */
    void hoverEnterEvent(QGraphicsSceneHoverEvent* ) override;

    /**
     * @brief hoverLeaveEvent
     * Adapts the color of the node to represent the hoverLeave event
     */
    void hoverLeaveEvent(QGraphicsSceneHoverEvent*) override;

private:
	/**
	 * Set primary and secondary color to the same color.
	 * @param color The new primary and secondary color.
	 */
	void setColors(const QColor& color);

	/**
	 * Set primary and secondary color to the provided colors.
	 * @param primary The new primary color.
	 * @param secondary The new secondary color.
	 */
	void setColors(const QColor& primary, const QColor& secondary);

	/**
	 * Set primary color to the provided color.
	 * @param primary The new primary color.
	 */
	void setPrimaryColor(const QColor& primary);

	/**
	 * Set secondary color to the provided color.
	 * @param secondary The new secondary color.
	 */
	void setSecondaryColor(const QColor& secondary);

    /**
     * @brief getStart
     * Calculates the start position for the name of the node in the visualisation such that it is positioned
     * in the middle of the node (if possible).
     * @param name Name of the Node
     * @return The start position of the node name in the visualisation of the node
     */
    int getStart( const QString& name );

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
    QString abbrev_;

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
