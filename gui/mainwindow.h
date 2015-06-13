#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "NetworkInstance.h"

namespace Ui {
class MainWindow;
}

class QLabel;

class Config;
class discretisationSelection;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow
     * Constructor of the Mainindow. Initialises the visibility of GUI components
     * @param parent
     */
    MainWindow(Config* config, QWidget *parent = 0);

    /**
     * @brief ~MainWindow()
     * Destructor of MainWindow
     */
    ~MainWindow();

private:

    /**
     * @brief loadQueriesToHistoryWindow
	 * Adds all queries contained in the QueryManager of the current NetworkInstance to the QueryHistory ListWidget
     * @param index Index of the NetworkInstance
     */
    void loadQueriesToHistoryWindow(const NetworkInstance& net);

    /**
     * @brief clearLabelsAndValueList
	 * Resets all labels and ListWidgets
     */
    void clearLabelsAndValueList();

    /**
     * @brief checkQueriesLeft
	 * Adapts the buttons controling loading of previous and subsequent query items depending on the current query index
     */
    void checkQueriesLeft();

    /**
     * @brief adaptQueryEvaluationButtons
     * @param show Flag controlling the appearnce of all button related to query evaluation
     */
    void adaptQueryEvaluationButtons(bool show);

    /**
     * @brief initaliseVisibility
	 * Initialises the visiblity of the GUI components at the start of the programme
     */
    void initaliseVisibility();

    /**
     * @brief checkEmptyList
	 * Checks if the given QListWidget is empty and adapts its visibility accordingly
     * @param widget Pointer to a QListWidget
     * @param label Pointer to a QLabel
     */
    void checkEmptyList(QListWidget* widget, QLabel* label);

    /**
     * @brief writeListWidget
	 * Adds the elements of vector vec to the given QListWidget and adapts the visiblity of both the QListWidget and the related QLabel
     * @param widget Pointer to a QListWidget
     * @param label Pointer to a QLabel
     * @param vec Reference to a vector containing QStrings
     */
    void writeListWidget(QListWidget *widget, QLabel *label, std::vector<QString> &vec);

    /**
     * @brief removeDuplicates
	 * Checks if the given QListWidget contains the given node
     * @param widget Pointer to a QListWidget
     * @param nodeName Name of the query node
     * @param index Position of the line in the QListWidgets that should be compared to the query name
	 * @return -1 if there is no duplicate, otherwise the index of the duplicate
     */
    int removeDuplicates(QListWidget *widget, QString nodeName, unsigned int index);

    /**
     * @brief loadSamples
	 * Trains the network according to the provided samples and the provided discretisation information.
     * @param samples
     * @param controlDiscret
     * @param index Index of the NetworkInstance
     */
    void loadSamples(const QString& samples, Discretiser& d, int index);

    /**
     * @brief visualise
	 * Create the network visualisation of the given network instance
     * @param index Index of the NetworkInstance
     */
    void visualise(int index);

    /**
     * @brief loadSif
	 * Loads a file in the SimpleInteractionFormat
     * @param filename Name of the file 
     * @param index Index of the NetworkInstance
     */
    void loadSif(QString filename, int index);

    /**
     * @brief loadNAorTGF
	 * Loads a NA or TGF file
     * @param filename Name of the file
     * @param index Index of the NetworkInstance
     */
    void loadNAorTGF(QString filename, int index);

    /**
     * @brief generateNetworkInstance
	 * Generates a new NetworkInstance object
     * @return The index of the generated NetworkInstance
     */
    int generateNetworkInstance();

    /**
     * @brief getVector
	 * Creates a vector containing the entries of the given QListWidget
     * @param widget Pointer to a QListWidget
     * @return Vector of the entries of the given QListWidget
     */
    std::vector<QString> getVector(QListWidget* widget);

    /**
     * @brief buildQuery
	 * Buils a complete query according to the selected query elements
     * @return A Query for CausalTrail
     */
    QString buildQuery();

    /**
     * @brief ui
	 * The Graphical User Interface
     */
    Ui::MainWindow *ui;

    /**
     * @brief networks
	 * Vector holding all active NetworkInstances
     */
    std::vector<NetworkInstance> networks;

	Config* config_;
	discretisationSelection* discretisationSelection_;

private slots:

	/**
	 * @brief checkAllEmpty()
	 * If all elements of a query are deleted, a new query will be triggered
	 * automatically
	 */
	void checkAllEmpty();

    /**
     * @brief discretiseSelection
     * @param samples name of the file containing the original observations
	 * @param deselected
     */
    void discretiseSelection(const QString& samples, const std::vector<uint>& deselected);

	/**
     * @brief on_actionClose_triggered
	 * Terminates the GUI 
     */
    void on_actionClose_triggered();

	/**
	 * Discretises the data currently configured
	 * in discretisationSelection_.
	 */
    void loadSamples();

    /**
     * @brief on_actionLoad_Samples_triggered
	 * Calls the loadSamples method
     */
    void on_actionLoad_Samples_triggered();

    /**
     * @brief on_actionAbout_triggered
	 * Shows information on Causal Trail
     */
    void on_actionAbout_triggered();

    /**
     * @brief on_actionHelp_triggered
	 * Shows the documentation of Causal Trails GUI
     */
    void on_actionHelp_triggered();

    /**
     * @brief on_tabWidget_tabCloseRequested
	 * Closes the tab with the given index and removes the corresponding NetworkInstance
     * @param index Index of the closed tab
     */
    void on_tabWidget_tabCloseRequested(int index);

    /**
     * @brief on_executeQueryButton_clicked
	 * Executes the current query and shows its result
     */
    void on_executeQueryButton_clicked();

    /**
	 * @brief on_actionDeleteNetwork_triggered
	 * Deletes the current network
     */
    void on_actionDeleteNetwork_triggered();

    /**
     * @brief on_actionNewNetwork_triggered
	 * Loads a new network
     */
    void on_actionNewNetwork_triggered();

    /**
     * @brief on_deleteQueryButton_clicked
	 * Clears the result labels
     */
    void on_deleteQueryButton_clicked();

    /**
     * @brief on_loadPreviousQueryButton_clicked
	 * Loads the previous query
     */
    void on_loadPreviousQueryButton_clicked();

    /**
     * @brief on_loadSuccessorQueryButton_clicked
	 * Loads the subsequent query
     */
    void on_loadSuccessorQueryButton_clicked();

    /**
     * @brief on_tabWidget_currentChanged
	 * Changes the network visualization, adapts the query history and all components related to query evaulation
     * @param index Index of tabTabwidget that is now selectet
     */
    void on_tabWidget_currentChanged(int index);

    /**
     * @brief on_actionLayout_triggered
	 * Relayouts the network
     */
    void on_actionLayout_triggered();

    /**
     * @brief Node_double_clicked
	 * Handles a double click on a node. 
     * @param id Identifier of the node upon whom the double click was executed
     * @param name Name of the node upon whom the double click was executed
     */
    void Node_double_clicked(uint id, QString name);

    /**
     * @brief Node_context
	 * Handles a right click on a node, generates the context menu
     * @param name Name of the node upon whom the right click was performed
     * @param id Identifier of the node upon whom the right click was performed
     * @param event QGraphicsSceneContextMenuEvent Pointer
     */
    void Node_context(QString name, uint id, QGraphicsSceneContextMenuEvent* event);

    /**
     * @brief Edge_context
	 * Handles a right click on an edge, generates the context menu
     * @param edge The edge for which the context menu was invoked
     * @param event QGraphicsSceneContextMenuEvent Pointer
     */
    void Edge_context(Edge* edge, QGraphicsSceneContextMenuEvent *event);

    /**
     * @brief context_Menu_QueryValue_Selected
	 * Adds the selected query value to the non intervention list widget
     * @param act QAction Pointer
     */
    void context_Menu_QueryValue_Selected(QAction* act);

    /**
     * @brief context_Menu_InterventionValue_Selected
	 * Adds the selected query value to the intervention list widget
     * @param act QAction Pointer
     */
    void context_Menu_InterventionValue_Selected(QAction *act);

    /**
     * @brief context_Menu_ConditionValue_Selected
	 * Adds the selected query value to the condition list widget
     * @param act QAction Pointer
     */
    void context_Menu_ConditionValue_Selected(QAction *act);

    /**
     * @brief context_Menu_ArgMax_Selected
	 * Adds the selected query value to the non intervention list widget. Sets the current query to be a MAP query.
     */
    void context_Menu_ArgMax_Selected();

    /**
     * @brief context_Menu_AddEdge_selected
	 * Initalises an edge addition
     */
    void context_Menu_AddEdge_selected();

    /**
     * @brief context_EdgeRemove_Selected
	 * Removes the selected edge
     */
    void context_EdgeRemove_Selected();

    /**
     * @brief context_Menu_ShowMatrix_selected
	 * Displays the probability matrix of the selected node
     */
    void context_Menu_ShowMatrix_selected();

    /**
     * @brief resizeEvent
	 * Handles the resizeEvent. Adapts the QDockWidget containing the network visualization to the biggest size possible
     */
    void resizeEvent(QResizeEvent *);

    /**
     * @brief on_Input_textChanged
	 * If the input value is cleared, all selected query items are deleted
     * @param arg1 New text
     */
    void on_Input_textChanged(const QString &arg1);

    /**
     * @brief on_newQuery_clicked
	 * Resets the GUI components to accept a new query
     */
    void on_newQuery_clicked();

    /**
     * @brief on_queryVariableList_itemDoubleClicked
	 * Removes the item that was double clicked from the query variable list
     * @param item QListWidgetItem Pointer
     */
    void on_queryVariableList_itemDoubleClicked(QListWidgetItem *item);

    /**
     * @brief on_conditionVariableList_itemDoubleClicked
	 * Removes the item that was double clicked from the condition variable list
     * @param item QListWidgetItem Pointer
     */
    void on_conditionVariableList_itemDoubleClicked(QListWidgetItem *item);

    /**
     * @brief on_interventionVariableList_itemDoubleClicked
	 * Removes the item that was double clicked from the intervention variable list
     * @param item QListWidgetItem Pointer
     */
    void on_interventionVariableList_itemDoubleClicked(QListWidgetItem *item);

    /**
     * @brief on_edgeAdditionsRemovalList_itemDoubleClicked
	 * Removes the item that was double clicked from the edge changes list
     * @param item QListWidgetItem Pointer
     */
    void on_edgeAdditionsRemovalList_itemDoubleClicked(QListWidgetItem *item);

    /**
     * @brief on_actionSaveSession_triggered
	 * Saves the current session
     */
    void on_actionSave_Session_triggered();

    /**
     * @brief on_actionLoad_Session_triggered
	 * Loads a previously saved session
     */
    void on_actionLoad_Session_triggered();

    /**
     * @brief on_actionCreate_Batchfile_triggered
	 * Creates a batchfile containing all queries that are in the current query history
     */
    void on_actionCreate_Batchfile_triggered();

    /**
     * @brief on_actionExecute_Batchfile_triggered
	 * Executes a previously saved batchile
     */
    void on_actionExecute_Batchfile_triggered();

    /**
     * @brief on_queryHistory_doubleClicked
	 * Reloads the selected query
     * @param index Item index of the item that was double clicked
     */
    void on_queryHistory_doubleClicked(const QModelIndex &index);

	/**
	 * @brief dataRejected
	 * Resets the network if the user did not select data in the dataview
	 */
    void dataRejected();
};

#endif // MAINWINDOW_H
