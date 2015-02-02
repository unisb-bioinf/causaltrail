#ifndef NETWORKINSTANCE_H
#define NETWORKINSTANCE_H

#include <QString>
#include <QListWidget>
#include "networkvis.h"
#include "querymanager.h"
#include "../core/NetworkController.h"


class NetworkInstance{
 public:
    /**
     * @brief NetworkInstance
     * Constructor of the NetworkInstance class
     */
    NetworkInstance();

    /**
     * @brief loadNetwork
     * Calls the appropiate network loading methods for the given filename
     * @param filename Name of the file to load
     */
    void loadNetwork(QString filename);

    /**
     * @brief visualize
     * Visualises the network represented by this class
     * @param tabwidget Pointer to the tabwidget containing the visualization
     */
    void visualize(QWidget* tabwidget);

    /**
     * @brief loadSamples
     * Load and discretise the specified samples according to the specified controlfile
     * @param filename Name of the file containing samples
     * @param controlfile Name of the file containing discretisation control information
     */
    void loadSamples(QString filename, QString controlfile);

    /**
     * @brief calculate
     * Calculates the given query
     * @param query Query to calculate
     * @return A pair of the resulting probability and a vector containing value assignments for MAP queries.
     */
    std::pair<float,std::vector<std::string>> calculate(QString query);

    /**
     * @brief isFirstQuery
     * @return True if the current query is the first one in the query list, false otherwise.
     */
    bool isFirstQuery();

    /**
     * @brief isLastQuery
     * @return True if the current query is the last one in the query list, false otherwise.
     */
    bool isLastQuery();

    /**
     * @brief isTrained
     * @return True if the network is trained, false otherwise
     */
    bool isTrained();

    /**
     * @brief getPreviousQueryItems
     * @return A vector containing the non intervention items of the previous query
     */
    std::vector<QString> &getPreviousQueryItems();

    /**
     * @brief getSubsequentQueryItems
     * @return A vector containing the non intervention items of the subsequent query
     */
    std::vector<QString> &getSubsequentQueryItems();

    /**
     * @brief getPreviousConditionItems
     * @return A vector containing the conditional items of the previous query
     */
    std::vector<QString> &getPreviousConditionItems();

    /**
     * @brief getSubsequentConditionItems
     * @return A vector containing the conditional items of the subsequent query
     */
    std::vector<QString> &getSubsequentConditionItems();

    /**
     * @brief getPreviousInterventionItems
     * @return A vector containing the interventional items of the previous query
     */
    std::vector<QString> &getPreviousInterventionItems();

    /**
     * @brief getSubsequentInterventionItems
     * @return A vector containing the interventional items of the subsequent query
     */
    std::vector<QString> &getSubsequentInterventionItems();

    /**
     * @brief getPreviousEdgeChangeItems
     * @return A vector containing the edge change items of the previous query
     */
    std::vector<QString> &getPreviousEdgeChangeItems();

    /**
     * @brief getSubsequentEdgeChangeItems
     * @return  A vector containing the edge change items of the subsequent query
     */
    std::vector<QString> &getSubsequentEdgeChangeItems();

    /**
     * @brief getNumberOfQueries
     * @return The number of stored queries
     */
    unsigned int getNumberOfQueries();

    /**
     * @brief getQuery
     * @param index The index of the desired query
     * @return The query at the given index
     */
    QString &getQuery(unsigned int index);

    /**
     * @brief getQueryItems
     * @param index The index of the desired query
     * @return A vector of the non intervention query items of the query with the given index
     */
    std::vector<QString>& getQueryItems(unsigned int index);

    /**
     * @param index The index of the desired query
     * @return A vector of the conditional query items of the query with the given index
     */
    std::vector<QString>& getConditionItems(unsigned int index);

    /**
     * @brief getInterventionItems
     * @param index The index of the desired query
     * @return A vector of the intervention items of the query with the given index
     */
    std::vector<QString>& getInterventionItems(unsigned int index);

    /**
     * @brief getEdgeAddRemItems
     * @param index The index of the desired query
     * @return A vector of the edge change items of the query with the given index
     */
    std::vector<QString>& getEdgeAddRemItems(unsigned int index);

    /**
     * @brief layout
     * Layout the current network again
     */
    void layout();

    /**
     * @brief getValues
     * @param id Identifier of the node of interest
     * @return Reference to a vector of all possible values of the node with the given identifier
     */
    const std::vector<std::string>& getValues(unsigned int id);

    /**
     * @brief removeHighlighting
     * Removes highlighting from all nodes and edges
     */
    void removeHighlighting();

    /**
     * @brief setArgMax
     * Sets the current query to a MAP/normal query
     * @param flag Bool indicating whether or not the current query is a MAP query or not
     */
    void setArgMax(bool flag);

    /**
     * @brief isArgMax
     * @return True if the current query is a MAP query, false otherwise
     */
    bool isArgMax();

    /**
     * @brief initialiseEdgeAddition
     * Initialises member variables to accept an edge additon
     */
    void initialiseEdgeAddition();

    /**
     * @brief initialiseEdgeRemoval
     * Initialises member variables to accept an edge removal
     */
    void initialiseEdgeRemoval();

    /**
     * @brief EdgeAddition
     * @return True if the network instance waits for an edge addition, false otherwise
     */
    bool EdgeAddition();

    /**
     * @brief NodeForEdgeAdditionSelected
     * Adds the given identifier as a node in the edge addition process
     * @param id Identifier of the selected node
     */
    void NodeForEdgeAdditionSelected(unsigned int id);

    /**
     * @brief EdgeRemoval
     * Assigns the given values to the member variables of this class to execute an edge removal
     * @param srcId Identifier of the source node
     * @param tarId Identifier of the target node
     * @param name1 Name of the source node
     * @param name2 Name of the target node
     */
    void EdgeRemoval(unsigned int srcId, unsigned int tarId, QString name1, QString name2);

    /**
     * @brief RemoveSelectedEdge
     * Removes the selected edge
     */
    void RemoveSelectedEdge();

    /**
     * @brief getNVSizeHint
     * @return QSize object of the network visualization
     */
    QSize getNVSizeHint();

    /**
     * @brief resizeNV
     * Resize the network visualization according to the given QSize object
     * @param qSize QSize object
     */
    void resizeNV(QSize qSize);

    /**
     * @brief setWidth
     * Set the width of the network visualization according to the given width
     * @param width Width of the network visualization
     */
    void setWidth(unsigned int width);

    /**
     * @brief getNodeGuiVec
     * @return Reference to a vector of pointers containing all NodeGui objects
     */
    const std::vector<NodeGui*>& getNodeGuiVec();

    /**
     * @brief setSelectedNode
     * @param id Identifier of the selected node
     * @param name Name of the selected node
     */
    void setSelectedNode(unsigned int id, QString name);

    /**
     * @brief doIntervention
     * Performs a doIntervention on the selected node
     */
    void doIntervention();

    /**
     * @brief restoreOriginalNetworkRepresentation
     * Restores the original network representation
     */
    void restoreOriginalNetworkRepresentation();

    /**
     * @brief getSelectedNodeName
     * @return The name of the selected node
     */
    QString getSelectedNodeName();

    /**
     * @brief removedEdgeSourceName
     * @return The name of the source node of the currently selected removed edge
     */
    QString removedEdgeSourceName();

    /**
     * @brief removedEdgeTargetName
     * @return The name of the target node of the currently selected removed edge
     */
    QString removedEdgeTargetName();

    /**
     * @brief getSelectedNodeID
     * @return Identifier of the currently selected node
     */
    unsigned int getSelectedNodeID();

    /**
     * @brief getEdgeVec
     * @return Reference to a vector of pointers containing all edge objects
     */
    const std::vector<Edge *> &getEdgeVec();

    /**
     * @brief removeDoIntervention
     * Removes the do intervention on the node with the given name
     * @param name Name of the node upon which the do intervention should be reversed
     */
    void removeDoIntervention(QString name);

    /**
     * @brief reverseEdgeRemoval
     * Reverse edge removal
     * @param source Name of the source node
     * @param target Name of the target node
     */
    void reverseEdgeRemoval(QString source, QString target);

    /**
     * @brief reverseEdgeAddition
     * Reverse edge addition
     * @param source Name of the source node
     * @param target Name of the target node
     */
    void reverseEdgeAddition(QString source, QString target);

    /**
     * @brief storeQuery
     * Stores the provided query, including all subitems.
     * @param query The completed query
     * @param queries Vector of all non intervention query items
     * @param conditions Vector of all condition query items
     * @param interventions Vector of all intervention query items
     * @param edgeChanges Vector of all edge Change operation items
     */
    void storeQuery(QString query, std::vector<QString> queries, std::vector<QString> conditions, std::vector<QString> interventions, std::vector<QString> edgeChanges);

    /**
     * @brief getRowCountOfCurrentProbMatrix
     * @return Number of rows of the probability matrix of the currently selected node
     */
    unsigned int getRowCountOfCurrentProbMatrix();

    /**
     * @brief getColCountOfCurrentProbMatrix
     * @return Number of columns of the probability matrix of the currently selected node
     */
    unsigned int getColCountOfCurrentProbMatrix();

    /**
     * @brief getProbabilityAt
     * @param row Row of interest
     * @param col Column of interest
     * @return Value at the specified position in the probability matrix of the currently selected node
     */
    float getProbabilityAt(unsigned int row, unsigned int col);

    /**
     * @brief getRowNamesOfProbabilityMatrix
     * @return Reference to a vector containing the row names of the probability matrix of the currently selected node
     */
    const std::vector<std::string>& getRowNamesOfProbabilityMatrix();

    /**
     * @brief getColNamesOfProbabilityMatrix
     * @return Reference to a vector containing the column names of the probability matrix of the currently selected node
     */
    const std::vector<std::string>& getColNamesOfProbabilityMatrix();

    /**
     * @brief getParentsOfSelectedNode
     * @return Vector of name(s) of the parent(s) of the currently selected node
     */
    std::vector<std::string> getParentsOfSelectedNode();

    /**
     * @brief getPreviousQuery
     * @return Reference to the previously asked query
     */
    QString &getPreviousQuery();

    /**
     * @brief getSubsequentQuery
     * @return Reference to the subsequent query
     */
    QString &getSubsequentQuery();

    /**
     * @brief getNumberOfParentsOfSelectedNode
     * @return Number of parents of the currently selected node
     */
    unsigned int getNumberOfParentsOfSelectedNode();

    /**
     * @brief colorNode
     * Sets the color of the given node to the given color
     * @param id Identifier of the node to which the color change should be applied
     * @param color New Color
     */
    void colorNode(unsigned int id, QColor color);

    /**
     * @brief removeNodeColor
     * Removes the given color from the node with the given name
     * @param name Name of the node to which the color change should be applied
     * @param color Color to be removed
     */
    void removeNodeColor(QString name, QColor color);

    /**
     * @brief checkEdgeAddition
     * @param id Identifier of the target node for the new edge
     * @return True if the edge can be added, false otherwise
     */
    bool checkEdgeAddition(unsigned int id);

    /**
     * @brief getNaOrTgf
     * @return The Name of the .NA or .TGF file
     */
    QString getNaOrTgf();

    /**
     * @brief getSif
     * @return The name of the .SIF file
     */
    QString getSif();

    /**
     * @brief getDataFile
     * @return Name of the file containing the samples
     */
    QString getDataFile();

    /**
     * @brief getDiscretisationControlFile
     * @return Name of the file containing discretisation control information
     */
    QString getDiscretisationControlFile();

    /**
     * @brief setNaOrTgf
     * @param filename Name of the .NA or .TGF file
     */
    void setNaOrTgf(QString& filename);

    /**
     * @brief setSif
     * @param filename Name of the .SIF file
     */
    void setSif(QString& filename);

    /**
     * @brief setDataFile
     * @param filename Name of the file containing samples
     */
    void setDataFile(QString& filename);

    /**
     * @brief setDiscretisationControlFile
     * @param filename Name of the file containing discretisation control information
     */
    void setDiscretisationControlFile(QString& filename);

    /**
     * @brief setQMA
     * @param qma Reference to a QueryManager object
     */
    void setQMA(QueryManager &qma);

    /**
     * @brief resetNetwork
     * Calls the reset network method of networkController. Reset all nodes to an empty state.
     * The network needs to be trained again after this method is executed
     */
    void resetNetwork();

private:

    /**
     * @brief visualizeNonInterventionNodes
     * Colors all non intervention nodes contained in the given vector
     * @param nonInterventions Reference to a vector containing all non intervention items of the current query
     */
    void visualizeNonInterventionNodes(std::vector<QString> &nonInterventions);

    /**
     * @brief visualizeCondition
     * Colors all condition nodes contained in the given vector
     * @param condition Reference to a vector containing all condition items of the current query
     */
    void visualizeCondition(std::vector<QString> &condition);

    /**
     * @brief visualizeInterventions
     * Colors all intervention nodes contained in the given vector
     * @param interventions Reference to a vector containing all intervention items of the current query
     */
    void visualizeInterventions(std::vector<QString>& interventions);

    /**
     * @brief visualizeEdgeChanges
     * Colors all edge changes contained in the given vector
     * @param edgeChanges Reference to a vector containing all edge change items of the current query
     */
    void visualizeEdgeChanges(std::vector<QString>& edgeChanges);

    /**
     * @brief nv_
     * Pointer to NetworkVis object
     */
    NetworkVis* nv_;

    /**
     * @brief nc_
     * NetworkController object
     */
    NetworkController nc_;

    /**
     * @brief qma_
     * QueryManager object
     */
    QueryManager qma_;

    /**
     * @brief trained_
     * True if the network is trained, false otherwise
     */
    bool trained_;

    /**
     * @brief argmax_
     * True if the current query is an MAP query
     */
    bool argmax_;

    /**
     * @brief remainingNodesForEdgeAddition_
     * Number of edges that must be selected for the edge addition
     */
    unsigned int remainingNodesForEdgeAddition_;

    /**
     * @brief id1_
     * Identifier of the edge source node
     */
    int id1_;

    /**
     * @brief id2_
     * Identifier of the edge target node
     */
    int id2_;

    /**
     * @brief name1_
     * Name of the edge source ndoe
     */
    QString name1_;

    /**
     * @brief name2_
     * Name of the edge target node
     */
    QString name2_;

    /**
     * @brief selectedNode_
     * Identifer of the selected node
     */
    unsigned int selectedNode_;

    /**
     * @brief selectedNodeName_
     * Name of the selected node
     */
    QString selectedNodeName_;

    /**
     * @brief naOrTgf_
     * Name of the .NA or .TGF file
     */
    QString naOrTgf_;

    /**
     * @brief sif_
     * Name of the .SIF file
     */
    QString sif_;

    /**
     * @brief dataFile_
     * Name of the file containing samples
     */
    QString dataFile_;

    /**
     * @brief discretisationControl_
     * Name of the file containing discretisation informaiton
     */
    QString discretisationControl_;

};

#endif // NETWORKINSTANCE_H
