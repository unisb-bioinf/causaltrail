#ifndef NETWORKVIS_H
#define NETWORKVIS_H

#include <QtWidgets/QGraphicsView>

/**
 * This class is based on the Elastic Nodes Example from the Qt documentation:
 * http://doc.qt.io/qt-5/qtwidgets-graphicsview-elasticnodes-example.html#graphwidget-class-definition
 * (Accessed 31.01.2015)
 */

class Edge;
class NodeGui;
class Network;

class NetworkVis : public QGraphicsView
{

    Q_OBJECT

public slots:
	/**
	 * Compute a graph layout for the network.
	 */
	void layoutGraph();

public:
    /**
     * Constructor of the networkVisualization class
     *
     * @param parent Pointer to the parent QWidget
     * @param net The network for which the visualisation should be created.
     */
    NetworkVis(QWidget *parent, const Network& net);

    /**
     * @brief getNode
     * @param id Identifier of the desired node
     * @return A pointer to the node with the given id
     */
    NodeGui* getNode(unsigned int id);

    /**
     * @brief doIntervention
     * @param id Identifier of the node upon which a do intervention should be performed / reversed
     * @param flag Bool indicating whether a do intervention should be performed or reversed
     */
    void doIntervention(unsigned int id, bool flag);

    /**
     * @brief removeEdge
     * Removes the edge from node id1 to node id2
     * @param id1 Identifier of the source node
     * @param id2 Identifier of the target node
     */
    void removeEdge(unsigned int id1, unsigned int id2);

    /**
     * @brief addEdge
     * @param id1 Identifier of the source node
     * @param id2 Identifier of the target node
     */
    void addEdge(unsigned int id1, unsigned int id2);

    /**
     * @brief removeNodeHighlighting
     * Removes the highlighting from all nodes in the network
     */
    void removeNodeHighlighting();

    /**
     * @brief removeEdgeHighlighting
     * Removes the highlighting from all edges in the network
     */
    void removeEdgeHighlighting();

    /**
     * @brief removeAdditionalEdges
     * Remove all additional edges
     */
    void removeAdditionalEdges();

    /**
     * @brief restoreEdges
     * Restore all edges to the original state
     */
    void restoreEdges();

    /**
     * @brief unDoDoInt
     * Remove all do interventions in the network
     */
    void unDoDoInt();

    /**
     * @brief restoreEdge
     * Restores the (previously deleted) edge from source to target
     * @param source Name of the source node
     * @param target Name of the target node
     */
    void restoreEdge(QString source, QString target);

    /**
     * @brief removeAdditionalEdge
     * Removes the previously added edge from source to target
     * @param source Name of the source node
     * @param target Name of the target node
     */
    void removeAdditionalEdge(QString source, QString target);

    /**
     * @brief getAddedEdgeIDs
     * @return A vector of pairs of identifiers for all added edges
     */
    std::vector<std::pair<unsigned int, unsigned int>> getAddedEdgeIDs();

    /**
     * @brief getRemovedEdgeIDs
     * @return A vector of pairs of identifiers for all removed edges
     */
    std::vector<std::pair<unsigned int, unsigned int>> getRemovedEdgeIDs();

    /**
     * @brief originalNodeState
     * Restores the original state for all nodes in the network
     */
    void originalNodeState();

	/**
	 * Write the visualization to an SVG file.
	 */
	void exportSVG(const QString& filename);

    signals:
	/**
	 * Signal that fires when a context menu for an edge was requested.
	 */
	void context(Edge*, QContextMenuEvent*);

	/**
	 * Signal that fires when a context menu for a node was requested.
	 */
	void context(NodeGui*, QContextMenuEvent*);

	/**
	 * Signal that fires when an node was double clicked.
	 */
	void doubleClick(NodeGui*);

protected:
	void contextMenuEvent(QContextMenuEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;

private:
	/**
	 * The network that is represented by this NetworkVis.
	 */
	const Network& net_;

    /**
     * @brief pointerVec_
     * Vector holding pointers to all NodeGui objects
     */
    std::vector<NodeGui*> pointerVec_;

    /**
     * @brief pointerVecEdges_
     * Vector holding pointers to all Edge objects
     */
    std::vector<Edge*> pointerVecEdges_;

    /**
     * @brief additionalEdges_
     * Vector holding pointers to all additional Edge objects
     */
    std::vector<Edge*> additionalEdges_;

    /**
     * @brief removedEdges_
     * Vector holding pointers to all removed Edge objects
     */
    std::vector<Edge*> removedEdges_;

    /**
     * @brief createLayout
     * Creates a grid layout for the current visualization
     * @param parent Pointer to the parent widget
     */
    void createLayout(QWidget *parent);

    /**
     * @brief createScene
     * Initialises the scence
     */
    void createScene();

    /**
     * Generates NodeGui objects for all nodes contained in the network
	 * represented by this NetworkVis object.
     */
    void loadNodes();

    /**
     * Generates Edges objects for all edges contained in the network
	 * represented by this NetworkVis object.
     */
    void loadEdges();

    /**
     * Calls zoom in and zoom out depending on whether the "+"-key or "-"-key is pressed
     *
     * @param event Pointer to QKeyEvent
     */
    void keyPressEvent(QKeyEvent *event) override;

    /**
     * Scales the current view depending on the given scaleFactor
     *
     * @param scaleFactor ScaleFactor to scale the view
     */
    void scaleView(qreal scaleFactor);

    /**
     * Calls zoom in and zoom out depending on the wheel direction
     *
     * @param event Pointer to QWheelEvent
     */
    void wheelEvent(QWheelEvent *event) override;

    /**
     * @brief zoomIn
     */
    void zoomIn();

    /**
     * @brief zoomOut
     */
    void zoomOut();

    /**
     * @brief shiftNodes
     * Shifts all nodes to the center of the visualization
     */
    void shiftNodes();

	/**
	 * Computes a graph layout using a force directed
	 * layout algorithm.
	 */
	void forceDirectedLayout();

    /**
     * Computes a graph layout using the Graphviz dot command.
     */
    bool dotLayout();

    /**
     * Helper function that writes the current network
     * in DOT format to a QIODevice.
     */
    void writeDot_(QIODevice& dev, const Network& network) const;

    /**
     * Helper function that reads a graph in DOT format
     * and sets the node positions to the found positions.
     */
    bool readDot_(const QByteArray& data);
};

#endif // NETWORKVIS_H
