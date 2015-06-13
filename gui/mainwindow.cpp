#include "mainwindow.h"

#include "ui_mainwindow.h"

#include "Config.h"
#include "datastorage.h"
#include "dataview.h"
#include "discretisationselection.h"
#include "edge.h"
#include "listwidgetmulticopy.h"
#include "matrixpopup.h"
#include "NodeGui.h"

#include "../core/Parser.h"

#include <QtCore/QDir>
#include <QtGui/QDesktopServices>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGraphicsSceneContextMenuEvent>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMessageBox>

MainWindow::MainWindow(Config* config, QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), config_(config)
{
	discretisationSelection_ = new discretisationSelection(this);
	connect(discretisationSelection_, SIGNAL(accepted()), this,
	        SLOT(loadSamples()));

	ui->setupUi(this);
	showMaximized();
	initaliseVisibility();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionClose_triggered() { close(); }

void MainWindow::on_actionHelp_triggered()
{
	QString pathToDoku = QDir::currentPath();
	pathToDoku.replace("/build/gui",
	                   "/Gui-Documentation-HTML/CausalTrailGuiDoku.html");
	pathToDoku.replace("/build",
	                   "/Gui-Documentation-HTML/CausalTrailGuiDoku.html");

	QDesktopServices::openUrl(QUrl(pathToDoku));
}

void MainWindow::checkQueriesLeft()
{
	if(networks.empty()) {
		return;
	}

	auto& currentNetwork = networks[ui->tabWidget->currentIndex()];
	ui->actionCreate_Batchfile->setEnabled(currentNetwork.getNumberOfQueries() > 0);
	ui->loadPreviousQueryButton->setEnabled(!currentNetwork.isFirstQuery());
	ui->loadPreviousQueryButton_2->setEnabled(!currentNetwork.isFirstQuery());
	ui->loadSuccessorQueryButton->setEnabled(!currentNetwork.isLastQuery());
	ui->loadSuccessorQueryButton_2->setEnabled(!currentNetwork.isLastQuery());
}

void MainWindow::initaliseVisibility()
{
	ui->dockWidget_2->setVisible(false);
	ui->dockWidget_5->setVisible(false);
	ui->dockWidget_3->setVisible(false);
	ui->queryVariableList->setVisible(false);
	ui->queryLabel->setVisible(false);
	ui->conditionVariableList->setVisible(false);
	ui->conditionLabel->setVisible(false);
	ui->interventionVariableList->setVisible(false);
	ui->interventionLabel->setVisible(false);
	ui->edgeAdditionsRemovalList->setVisible(false);
	ui->edgeAdReLabel->setVisible(false);
	ui->actionSave_Session->setEnabled(false);
}

void MainWindow::checkAllEmpty()
{
	if(ui->queryVariableList->count() == 0 &&
	   ui->conditionVariableList->count() == 0 &&
	   ui->interventionVariableList->count() == 0 &&
	   ui->edgeAdditionsRemovalList->count() == 0) {
		on_newQuery_clicked();
	}

	ui->Input->setFocus();
}

void MainWindow::checkEmptyList(QListWidget* widget, QLabel* label)
{
	if(widget->count() == 0) {
		widget->setVisible(false);
		label->setVisible(false);
	}
}

int MainWindow::removeDuplicates(QListWidget* widget, QString nodeName,
                                 unsigned int index)
{
	for(int i = 0; i < widget->count(); i++) {
		if(nodeName == widget->item(i)->text().split(" ")[index]) {
			return i;
		}
	}

	return -1;
}

std::vector<QString> MainWindow::getVector(QListWidget* widget)
{
	std::vector<QString> temp;
	for(int i = 0; i < widget->count(); i++) {
		temp.push_back(widget->item(i)->text());
	}

	return temp;
}

void MainWindow::writeListWidget(QListWidget* widget, QLabel* label,
                                 std::vector<QString>& vec)
{
	widget->clear();

	if(vec.empty()) {
		widget->setVisible(false);
		label->setVisible(false);
	} else {
		widget->setVisible(true);
		label->setVisible(true);
	}

	for(auto item : vec) {
		widget->addItem(item);
	}
}

int MainWindow::generateNetworkInstance()
{
	networks.push_back(NetworkInstance());
	ui->tabWidget->addTab(new QWidget,
	                      "New Tab " + QString::number(ui->tabWidget->count()));
	int index = ui->tabWidget->count() - 1;
	ui->tabWidget->setCurrentIndex(index);
	return index;
}

void MainWindow::loadNAorTGF(QString filename, int index)
{
	ui->Output->addItem("File: " + filename + " opened");
	networks[index].loadNetwork(filename);
	networks[index].setNaOrTgf(filename);
	QStringList list = filename.split("/");
	auto name = list[list.size() - 1];
	ui->tabWidget->setTabText(index, name);
}

void MainWindow::loadSif(QString filename, int index)
{
	ui->Output->addItem("File: " + filename + " opened");
	networks[index].loadNetwork(filename);
	networks[index].setSif(filename);
}

void MainWindow::visualise(int index)
{
	networks[index].visualize(ui->tabWidget->widget(index));
	ui->actionLoad_Samples->setEnabled(true);
	ui->actionLayout->setEnabled(true);
	ui->tabWidget->widget(index)->setFocus();
	for(const NodeGui* ng : networks[index].getNodeGuiVec()) {
		connect(ng, SIGNAL(doubleClick(uint, QString)), this,
		        SLOT(Node_double_clicked(uint, QString)));
		connect(
		    ng, SIGNAL(context(QString, uint, QGraphicsSceneContextMenuEvent*)),
		    this,
		    SLOT(Node_context(QString, uint, QGraphicsSceneContextMenuEvent*)));
	}
	for(Edge* edge : networks[index].getEdgeVec()) {
		connect(
		    edge, SIGNAL(context(Edge*, QGraphicsSceneContextMenuEvent*)),
		    this,
		    SLOT(Edge_context(Edge*, QGraphicsSceneContextMenuEvent*)));
	}
}

void MainWindow::loadSamples()
{
	try {
		loadSamples(discretisationSelection_->samples(),
		            discretisationSelection_->control(),
		            discretisationSelection_->index());
	} catch(std::invalid_argument& e) {
		ui->Output->addItem(e.what());
		adaptQueryEvaluationButtons(false);
		networks[discretisationSelection_->index()].resetNetwork();
	}
	ui->Output->scrollToBottom();
}

void MainWindow::loadSamples(const QString& samples,
                             const Discretiser::Discretisations& control,
                             int index)
{
	ui->Output->addItem("Reading samples: " + samples);
	networks[index].loadSamples(samples, control);
	networks[index].setDataFile(samples);
	adaptQueryEvaluationButtons(true);
	on_newQuery_clicked();
}

void MainWindow::discretiseSelection(const QString& samples,
                                     const std::vector<uint>& deselected)
{
	int index = ui->tabWidget->currentIndex();
	networks[index].setDeselectedSamples(deselected);
	QMessageBox boxDisc;
	boxDisc.setIcon(QMessageBox::Question);
	boxDisc.setText("Please specify discretisation information source.");
	QPushButton* file =
	    boxDisc.addButton("Load from File", QMessageBox::ActionRole);
	QPushButton* choose =
	    boxDisc.addButton("Choose Methods", QMessageBox::ActionRole);
	boxDisc.exec();
	try {
		if(boxDisc.clickedButton() == file) {
			QString control = QFileDialog::getOpenFileName(
			    this, tr("Open txt file containing discretisation information"),
			    config_->dataDir(), "*.txt");
			if(!control.isNull()) {
				loadSamples(samples,
				            Discretiser::loadControlFile(control.toStdString()),
				            index);
			} else {
				throw std::invalid_argument(
				    "Discretisation information is not specified");
			}
		} else {
			if(boxDisc.clickedButton() == choose) {
				discretisationSelection_->show(config_->dataDir(), samples,
				                               index);
			} else {
				throw std::invalid_argument(
				    "Discretisation information is not specified");
			}
		}
	} catch(std::exception& e) {
		ui->Output->addItem(e.what());
		adaptQueryEvaluationButtons(false);
		networks[index].resetNetwork();
	}
	ui->Output->scrollToBottom();
}

void MainWindow::dataRejected()
{
	ui->Output->addItem("No samples loaded");
	adaptQueryEvaluationButtons(false);
	int index = ui->tabWidget->currentIndex();
	networks[index].resetNetwork();
	ui->Output->scrollToBottom();
}

void MainWindow::on_actionLoad_Samples_triggered()
{
	int index = ui->tabWidget->currentIndex();
	try {
		QString samples = QFileDialog::getOpenFileName(
		    this, tr("Open txt file containing samples"), config_->dataDir(),
		    "*.txt");
		if(samples.isNull()) {
			throw std::invalid_argument(
			    "No file containing samples specified!");
		}
		QMessageBox boxSamples;
		boxSamples.setIcon(QMessageBox::Question);
		boxSamples.setText("Do you want to view the data?");
		QPushButton* yes = boxSamples.addButton("Yes", QMessageBox::ActionRole);
		boxSamples.addButton("No", QMessageBox::ActionRole);
		boxSamples.exec();
		if(boxSamples.clickedButton() == yes) {
			// View -> Call a new window
			dataview* dView = new dataview(0, samples);
			dView->setWindowTitle("Data contained in " + samples);
			connect(
			    dView, SIGNAL(dataAccepted(const QString&, std::vector<uint>)),
			    this,
			    SLOT(discretiseSelection(const QString&, std::vector<uint>)));
			connect(dView, SIGNAL(rejected()), this, SLOT(dataRejected()));
			dView->show();
		} else {
			std::vector<unsigned int> temp;
			discretiseSelection(samples, temp);
		}
	} catch(std::exception& e) {
		ui->Output->addItem(e.what());
		adaptQueryEvaluationButtons(false);
		networks[index].resetNetwork();
	}
	ui->Output->scrollToBottom();
}

void MainWindow::on_actionAbout_triggered()
{
	QMessageBox box;
	box.setIcon(QMessageBox::Information);
	box.setText("CausalTrail");
	box.setInformativeText("A statistical tool to performe inference in causal "
	                       "bayesian networks using Do-Calculus.");
	box.exec();
}

void MainWindow::on_executeQueryButton_clicked()
{
	QString query_ = buildQuery();
	if(query_ != "?") {
		ui->Input->setText(query_);
	}

	NetworkInstance& currentNetwork = networks[ui->tabWidget->currentIndex()];

	std::pair<float, std::vector<std::string>> result;
	try {
		result = currentNetwork.calculate(ui->Input->text().toStdString());
	} catch(std::invalid_argument& e) {
		ui->Output->addItem(e.what());
		ui->Output->scrollToBottom();
		return;
	}

	ui->Output->addItem(ui->Input->text() + ": " +
	                    QString::number(result.first));
	ui->probLabel->setVisible(true);
	ui->probResultLabel->setText(QString::number(result.first));

	QString temp = "";
	for(unsigned int i = 0; i < result.second.size(); i++) {
		temp += ui->queryVariableList->item(i)->text().split(" ")[1] + "=" +
		        QString::fromStdString(result.second[i]) + " ";
	}

	if(temp != "") {
		ui->valueLabel->setVisible(true);
		ui->Output->addItem(temp);
		ui->valueMaxLabel->setText(temp);
	}

	currentNetwork.storeQuery(ui->Input->text(),
	                          getVector(ui->queryVariableList),
	                          getVector(ui->conditionVariableList),
	                          getVector(ui->interventionVariableList),
	                          getVector(ui->edgeAdditionsRemovalList));

	loadQueriesToHistoryWindow(currentNetwork);
	checkQueriesLeft();
	query_ = "";
	currentNetwork.removeHighlighting();
	ui->Output->scrollToBottom();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
	networks.erase(networks.begin() + index);
	ui->tabWidget->removeTab(index);
}

void MainWindow::on_actionDeleteNetwork_triggered()
{
	if(!networks.empty()) {
		int index = ui->tabWidget->currentIndex();
		networks.erase(networks.begin() + index);
		ui->tabWidget->removeTab(index);
	}
}

void MainWindow::on_actionNewNetwork_triggered()
{
	if(ui->deleteQueryButton->isEnabled()) {
		on_deleteQueryButton_clicked();
	}
	QString filename = QFileDialog::getOpenFileName(
	    this, tr("Load network file"), config_->dataDir(), "*.tgf *.na");
	if(filename != "") {
		try {
			int index = generateNetworkInstance();
			loadNAorTGF(filename, index);
			if(filename.endsWith(".na")) {
				filename = QFileDialog::getOpenFileName(
				    this, tr("Load sif file"),
				    QFileInfo(filename).absolutePath(), "*.sif");
				loadSif(filename, index);
			}
			visualise(index);

		} catch(std::exception& e) {
			ui->Output->addItem(e.what());
		}
	} else {
		ui->Output->addItem("No file containing network data specified.");
	}
	ui->Output->scrollToBottom();
}

void MainWindow::on_deleteQueryButton_clicked()
{
	ui->probLabel->setVisible(false);
	ui->valueLabel->setVisible(false);
	ui->probResultLabel->clear();
	ui->valueMaxLabel->clear();
}

void MainWindow::on_loadPreviousQueryButton_clicked()
{
	on_deleteQueryButton_clicked();
	NetworkInstance& currentNetwork = networks[ui->tabWidget->currentIndex()];
	currentNetwork.removeHighlighting();
	currentNetwork.restoreOriginalNetworkRepresentation();
	ui->Input->setText(currentNetwork.getPreviousQuery());
	writeListWidget(ui->queryVariableList, ui->queryLabel,
	                currentNetwork.getPreviousQueryItems());
	writeListWidget(ui->conditionVariableList, ui->conditionLabel,
	                currentNetwork.getPreviousConditionItems());
	writeListWidget(ui->interventionVariableList, ui->interventionLabel,
	                currentNetwork.getPreviousInterventionItems());
	writeListWidget(ui->edgeAdditionsRemovalList, ui->edgeAdReLabel,
	                currentNetwork.getPreviousEdgeChangeItems());
	ui->queryLabel->setText("Calculating probability of");
	checkQueriesLeft();
}

void MainWindow::on_loadSuccessorQueryButton_clicked()
{
	on_deleteQueryButton_clicked();
	NetworkInstance& currentNetwork = networks[ui->tabWidget->currentIndex()];
	currentNetwork.removeHighlighting();
	currentNetwork.restoreOriginalNetworkRepresentation();
	ui->Input->setText(currentNetwork.getSubsequentQuery());
	writeListWidget(ui->queryVariableList, ui->queryLabel,
	                currentNetwork.getSubsequentQueryItems());
	writeListWidget(ui->conditionVariableList, ui->conditionLabel,
	                currentNetwork.getSubsequentConditionItems());
	writeListWidget(ui->interventionVariableList, ui->interventionLabel,
	                currentNetwork.getSubsequentInterventionItems());
	writeListWidget(ui->edgeAdditionsRemovalList, ui->edgeAdReLabel,
	                currentNetwork.getSubsequentEdgeChangeItems());
	ui->queryLabel->setText("Calculating probability of");
	checkQueriesLeft();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
	NetworkInstance& currentNetwork = networks[index];
	checkQueriesLeft();

	if(!networks.empty()) {
		if(currentNetwork.isTrained()) {
			adaptQueryEvaluationButtons(true);
			currentNetwork.restoreOriginalNetworkRepresentation();
			loadQueriesToHistoryWindow(currentNetwork);
		} else {
			adaptQueryEvaluationButtons(false);
		}
	} else {
		ui->actionLoad_Samples->setEnabled(false);
		ui->actionLayout->setEnabled(false);
		ui->actionSave_Session->setEnabled(false);
		ui->actionLoad_Session->setEnabled(true);
		ui->actionExecute_Batchfile->setEnabled(false);
		ui->actionCreate_Batchfile->setEnabled(false);
	}

	clearLabelsAndValueList();
}

void MainWindow::on_actionLayout_triggered()
{
	int index = ui->tabWidget->currentIndex();
	networks[index].layout();
}

void MainWindow::Node_double_clicked(uint id, QString name)
{
	NetworkInstance& currentNetwork = networks[ui->tabWidget->currentIndex()];

	currentNetwork.setSelectedNode(id, name);
	if(!currentNetwork.EdgeAddition()) {
		return;
	}

	if(currentNetwork.checkEdgeAddition(id)) {
		ui->edgeAdReLabel->setVisible(true);
		ui->edgeAdditionsRemovalList->setVisible(true);
		currentNetwork.NodeForEdgeAdditionSelected(id);
		ui->edgeAdditionsRemovalList->addItem(
		    "+ " + currentNetwork.getSelectedNodeName() + " " + name);
		if(!currentNetwork.EdgeAddition()) {
			ui->Input->setFocus();
		}
	} else {
		ui->Output->addItem("This edge would induce a cycle. Therefore, it "
		                    "can not be added!");
		ui->Output->scrollToBottom();
	}
}

void MainWindow::on_newQuery_clicked()
{
	ui->deleteQueryButton->click();
	clearLabelsAndValueList();
	if(!networks.empty()) {
		int index = ui->tabWidget->currentIndex();
		networks[index].removeHighlighting();
		networks[index].restoreOriginalNetworkRepresentation();
		networks[index].setArgMax(false);
	}

	ui->queryLabel->setVisible(true);
	ui->queryLabel->setText("Right click on a Node to build a query");
	ui->queryVariableList->setVisible(false);
	ui->conditionVariableList->setVisible(false);
	ui->conditionLabel->setVisible(false);
	ui->interventionVariableList->setVisible(false);
	ui->interventionLabel->setVisible(false);
	ui->edgeAdditionsRemovalList->setVisible(false);
	ui->edgeAdReLabel->setVisible(false);
	ui->Input->clear();
}

void MainWindow::Node_context(QString name, uint id,
                              QGraphicsSceneContextMenuEvent* event)
{
	NetworkInstance& currentNetwork = networks[ui->tabWidget->currentIndex()];
	QMenu* menu = new QMenu;
	if(currentNetwork.isTrained()) {
		const auto& values = currentNetwork.getValues(id);
		currentNetwork.setSelectedNode(id, name);
		QMenu* submenu;
		// Non-Intervention Query
		if(ui->queryVariableList->count() == 0 || !currentNetwork.isArgMax()) {
			submenu = menu->addMenu("Probability of");
			for(auto& value : values) {
				submenu->addAction(QString::fromStdString(value));
				connect(submenu, SIGNAL(triggered(QAction*)), this,
				        SLOT(context_Menu_QueryValue_Selected(QAction*)),
				        Qt::UniqueConnection);
			}
		}

		// Arg-Max
		if((ui->queryVariableList->count() == 0) || currentNetwork.isArgMax()) {
			connect(menu->addAction("Arg Max"), SIGNAL(triggered()), this,
			        SLOT(context_Menu_ArgMax_Selected()), Qt::UniqueConnection);
		}

		// Condition
		submenu = menu->addMenu("Condition on");
		for(auto& value : values) {
			submenu->addAction(QString::fromStdString(value));
			connect(submenu, SIGNAL(triggered(QAction*)), this,
			        SLOT(context_Menu_ConditionValue_Selected(QAction*)),
			        Qt::UniqueConnection);
		}

		menu->addSeparator();

		// Do-Int
		submenu = menu->addMenu("Set value to");
		for(auto& value : values) {
			submenu->addAction(QString::fromStdString(value));
			connect(submenu, SIGNAL(triggered(QAction*)), this,
			        SLOT(context_Menu_InterventionValue_Selected(QAction*)),
			        Qt::UniqueConnection);
		}
		// Add Edge To
		connect(menu->addAction("Add Edge to ..."), SIGNAL(triggered()), this,
		        SLOT(context_Menu_AddEdge_selected()), Qt::UniqueConnection);

		menu->addSeparator();

		// Show Probability Matrix
		connect(menu->addAction("Show CPT"), SIGNAL(triggered()), this,
		        SLOT(context_Menu_ShowMatrix_selected()), Qt::UniqueConnection);

	} else {
		menu->addAction("The network is not trained");
	}
	menu->popup(event->screenPos());
	ui->Input->setFocus();
}

void MainWindow::Edge_context(Edge* edge, QGraphicsSceneContextMenuEvent* event)
{
	NetworkInstance& currentNetwork = networks[ui->tabWidget->currentIndex()];
	currentNetwork.EdgeRemoval(edge->sourceNode()->getID(), edge->destNode()->getID());
	QMenu* menu = new QMenu;
	if(currentNetwork.isTrained()) {
		connect(menu->addAction("Remove Edge"), SIGNAL(triggered()), this,
		        SLOT(context_EdgeRemove_Selected()), Qt::UniqueConnection);
	} else {
		menu->addAction("The network is not trained");
	}
	menu->popup(event->screenPos());
}

void MainWindow::adaptQueryEvaluationButtons(bool show)
{
	ui->dockWidget_2->setVisible(show);
	ui->dockWidget_5->setVisible(show);
	ui->dockWidget_3->setVisible(show);
	ui->dockWidgetContents_4->setEnabled(show);
	ui->dockWidgetContents_2->setEnabled(show);
	ui->executeQueryButton_2->setEnabled(show);
	ui->deleteQueryButton_2->setEnabled(show);
	ui->actionSave_Session->setEnabled(show);
	ui->actionExecute_Batchfile->setEnabled(show);
}

void MainWindow::clearLabelsAndValueList()
{
	ui->probResultLabel->clear();
	ui->valueMaxLabel->clear();
	ui->conditionVariableList->clear();
	ui->interventionVariableList->clear();
	ui->edgeAdditionsRemovalList->clear();
	ui->queryVariableList->clear();
	ui->valueLabel->setVisible(false);
	ui->probLabel->setVisible(false);
	ui->conditionVariableList->setVisible(false);
	ui->interventionVariableList->setVisible(false);
	ui->edgeAdditionsRemovalList->setVisible(false);
	ui->queryVariableList->setVisible(false);
	ui->queryLabel->setText("Right click on a node to build a query");
	ui->conditionLabel->setVisible(false);
	ui->interventionLabel->setVisible(false);
	ui->edgeAdReLabel->setVisible(false);
}

void MainWindow::context_Menu_QueryValue_Selected(QAction* act)
{
	NetworkInstance& currentNetwork = networks[ui->tabWidget->currentIndex()];
	ui->queryLabel->setText("Calculating probability of");
	int duplicates = removeDuplicates(ui->interventionVariableList,
	                                  currentNetwork.getSelectedNodeName(), 0);
	if(duplicates != -1) {
		on_interventionVariableList_itemDoubleClicked(
		    ui->interventionVariableList->item(duplicates));
	}
	duplicates = removeDuplicates(ui->queryVariableList,
	                              currentNetwork.getSelectedNodeName(), 0);
	if(duplicates != -1) {
		on_queryVariableList_itemDoubleClicked(
		    ui->queryVariableList->item(duplicates));
	}
	duplicates = removeDuplicates(ui->queryVariableList,
	                              currentNetwork.getSelectedNodeName(), 1);
	if(duplicates != -1) {
		on_queryVariableList_itemDoubleClicked(
		    ui->queryVariableList->item(duplicates));
	}
	ui->queryVariableList->addItem(currentNetwork.getSelectedNodeName() +
	                               " = " + act->text());
	ui->queryVariableList->setVisible(true);
	ui->queryLabel->setVisible(true);
	QColor queryColor;
	queryColor.setHsv(100, 30, 250);
	currentNetwork.colorNode(currentNetwork.getSelectedNodeID(), queryColor);
	currentNetwork.removeHighlighting();
}

void MainWindow::context_Menu_ArgMax_Selected()
{
	NetworkInstance& currentNetwork = networks[ui->tabWidget->currentIndex()];
	currentNetwork.setArgMax(true);
	ui->queryLabel->setText(
	    "Calculating the most probable value assignment for");
	int duplicates = removeDuplicates(ui->queryVariableList,
	                                  currentNetwork.getSelectedNodeName(), 1);
	if(duplicates != -1) {
		on_queryVariableList_itemDoubleClicked(
		    ui->queryVariableList->item(duplicates));
	}
	duplicates = removeDuplicates(ui->interventionVariableList,
	                              currentNetwork.getSelectedNodeName(), 0);
	if(duplicates != -1) {
		on_interventionVariableList_itemDoubleClicked(
		    ui->interventionVariableList->item(duplicates));
	}
	duplicates = removeDuplicates(ui->conditionVariableList,
	                              currentNetwork.getSelectedNodeName(), 0);
	if(duplicates != -1) {
		on_conditionVariableList_itemDoubleClicked(
		    ui->conditionVariableList->item(duplicates));
	}
	ui->queryVariableList->addItem("argmax " +
	                               currentNetwork.getSelectedNodeName());
	ui->queryLabel->setVisible(true);
	ui->queryVariableList->setVisible(true);
	QColor queryColor;
	queryColor.setHsv(100, 30, 250);
	currentNetwork.colorNode(currentNetwork.getSelectedNodeID(), queryColor);
	currentNetwork.removeHighlighting();
}

void MainWindow::context_Menu_ConditionValue_Selected(QAction* act)
{
	NetworkInstance& currentNetwork = networks[ui->tabWidget->currentIndex()];
	ui->conditionLabel->setText("given that");
	int duplicates = removeDuplicates(ui->conditionVariableList,
	                                  currentNetwork.getSelectedNodeName(), 0);
	if(duplicates != -1) {
		on_conditionVariableList_itemDoubleClicked(
		    ui->conditionVariableList->item(duplicates));
	}
	ui->conditionVariableList->addItem(currentNetwork.getSelectedNodeName() +
	                                   " = " + act->text());
	ui->conditionLabel->setVisible(true);
	ui->conditionVariableList->setVisible(true);
	QColor queryColor;
	queryColor.setHsv(20, 30, 250);
	currentNetwork.colorNode(currentNetwork.getSelectedNodeID(), queryColor);
	currentNetwork.removeHighlighting();
}

void MainWindow::context_Menu_InterventionValue_Selected(QAction* act)
{
	NetworkInstance& currentNetwork = networks[ui->tabWidget->currentIndex()];
	ui->interventionLabel->setText("setting");
	int duplicates = removeDuplicates(ui->interventionVariableList,
	                                  currentNetwork.getSelectedNodeName(), 0);
	if(duplicates != -1) {
		on_interventionVariableList_itemDoubleClicked(
		    ui->interventionVariableList->item(duplicates));
	}
	duplicates = removeDuplicates(ui->queryVariableList,
	                              currentNetwork.getSelectedNodeName(), 0);
	if(duplicates != -1) {
		on_queryVariableList_itemDoubleClicked(
		    ui->queryVariableList->item(duplicates));
	}
	ui->interventionVariableList->addItem(currentNetwork.getSelectedNodeName() +
	                                      " = " + act->text());
	ui->interventionLabel->setVisible(true);
	ui->interventionVariableList->setVisible(true);
	currentNetwork.doIntervention();
	currentNetwork.removeHighlighting();
}

void MainWindow::context_Menu_AddEdge_selected()
{
	NetworkInstance& currentNetwork = networks[ui->tabWidget->currentIndex()];
	ui->edgeAdReLabel->setText("with respect to");
	currentNetwork.initialiseEdgeAddition();
	currentNetwork.NodeForEdgeAdditionSelected(
	    currentNetwork.getSelectedNodeID());
	currentNetwork.removeHighlighting();
}

void MainWindow::context_EdgeRemove_Selected()
{
	NetworkInstance& currentNetwork = networks[ui->tabWidget->currentIndex()];
	ui->edgeAdReLabel->setText("with respect to");
	ui->edgeAdReLabel->setVisible(true);
	ui->edgeAdditionsRemovalList->setVisible(true);
	ui->edgeAdditionsRemovalList->addItem(QString("- %1 %2").arg(
		QString::fromStdString(currentNetwork.removedEdgeSourceName()),
		QString::fromStdString(currentNetwork.removedEdgeTargetName())
	));
	currentNetwork.RemoveSelectedEdge();
	currentNetwork.removeHighlighting();
}

void MainWindow::context_Menu_ShowMatrix_selected()
{
	NetworkInstance& currentNetwork = networks[ui->tabWidget->currentIndex()];

	matrixPopup* mp = new matrixPopup();
	mp->setWindowTitle("CPT of " + currentNetwork.getSelectedNodeName());

	const unsigned int rowCount =
	    currentNetwork.getRowCountOfCurrentProbMatrix();
	const unsigned int colCount =
	    currentNetwork.getColCountOfCurrentProbMatrix();
	const unsigned int parentCount =
	    currentNetwork.getNumberOfParentsOfSelectedNode();
	mp->initaliseTable(rowCount + 1, colCount + parentCount);

	mp->setRowNames(currentNetwork.getRowNamesOfProbabilityMatrix());
	mp->setColNames(currentNetwork.getColNamesOfProbabilityMatrix(),
	                parentCount);
	mp->setParentNames(currentNetwork.getParentsOfSelectedNode());

	for(unsigned int row = 0; row < rowCount; row++) {
		for(unsigned int col = 0; col < colCount; col++) {
			mp->addData(row + 1, col + parentCount,
			            currentNetwork.getProbabilityAt(row, col));
		}
	}
	mp->adjust();
	mp->show();
	currentNetwork.removeHighlighting();
}

void MainWindow::on_Input_textChanged(const QString& arg1)
{
	if(!networks.empty() && arg1 == "") {
		on_deleteQueryButton_clicked();
		clearLabelsAndValueList();
	}
}

QString MainWindow::buildQuery()
{
	QString query = "?";
	if(networks[ui->tabWidget->currentIndex()].isArgMax()) {
		query += +" argmax (";
		for(int i = 0; i < ui->queryVariableList->count(); i++) {
			query += " " + ui->queryVariableList->item(i)->text().split(" ")[1];
		}
		query = query + " )";
	} else {
		for(int i = 0; i < ui->queryVariableList->count(); i++) {
			query += " " + ui->queryVariableList->item(i)->text();
		}
	}
	for(int j = 0; j < ui->conditionVariableList->count(); j++) {
		query += " | " + ui->conditionVariableList->item(j)->text();
	}

	for(int h = 0; h < ui->interventionVariableList->count(); h++) {
		query += " ! do " + ui->interventionVariableList->item(h)->text();
	}

	for(int k = 0; k < ui->edgeAdditionsRemovalList->count(); k++) {
		query += " ! " + ui->edgeAdditionsRemovalList->item(k)->text();
	}

	return query;
}

void MainWindow::on_queryVariableList_itemDoubleClicked(QListWidgetItem* item)
{
	int index = ui->tabWidget->currentIndex();
	QColor queryColor;
	queryColor.setHsv(100, 30, 250);
	QStringList list = item->text().split(" ");
	if(list[0] == "argmax") {
		networks[index].removeNodeColor(list[1], queryColor);
	} else {
		networks[index].removeNodeColor(list[0], queryColor);
	}
	delete item;
	checkEmptyList(ui->queryVariableList, ui->queryLabel);
	checkAllEmpty();
}

void
MainWindow::on_conditionVariableList_itemDoubleClicked(QListWidgetItem* item)
{
	int index = ui->tabWidget->currentIndex();
	QColor queryColor;
	queryColor.setHsv(20, 30, 250);
	networks[index].removeNodeColor(item->text().split(" ")[0], queryColor);
	delete item;
	checkEmptyList(ui->conditionVariableList, ui->conditionLabel);
	checkAllEmpty();
}

void
MainWindow::on_interventionVariableList_itemDoubleClicked(QListWidgetItem* item)
{
	int index = ui->tabWidget->currentIndex();
	networks[index].removeDoIntervention(item->text().split(" ")[0]);
	delete item;
	checkEmptyList(ui->interventionVariableList, ui->interventionLabel);
	checkAllEmpty();
}

void
MainWindow::on_edgeAdditionsRemovalList_itemDoubleClicked(QListWidgetItem* item)
{
	int index = ui->tabWidget->currentIndex();
	QStringList splitList = item->text().split(" ");
	if(splitList[0] == "-") {
		networks[index].reverseEdgeRemoval(splitList[1], splitList[2]);
	} else {
		networks[index].reverseEdgeAddition(splitList[1], splitList[2]);
	}
	delete item;
	checkEmptyList(ui->edgeAdditionsRemovalList, ui->edgeAdReLabel);
	checkAllEmpty();
}

void MainWindow::on_actionSave_Session_triggered()
{
	dataStorage dataStore;
	QFileDialog dialog;
	dialog.setDefaultSuffix("cts");
	dialog.exec();
	QString filename = dialog.selectedFiles()[0];
	if(!filename.isNull()) {
		dataStore.saveSession(networks, filename);
		ui->Output->addItem("Session saved");
	} else {
		ui->Output->addItem("No file specified. The Session was not stored!");
	}
	ui->Output->scrollToBottom();
}

void MainWindow::on_actionLoad_Session_triggered()
{
	dataStorage dataStore;
	QString filename = QFileDialog::getOpenFileName(
	    this, tr("Load session file"), config_->dataDir(), "*.cts");
	int index = 0;
	if(filename != "") {
		dataStore.loadSession(filename);
		for(unsigned int i = 0; i < dataStore.getNumberOfLoadedNetworks();
		    i++) {
			index = generateNetworkInstance();
			loadNAorTGF(dataStore.getNAorTGf(i), index);
			if(dataStore.getNAorTGf(i).endsWith(".na")) {
				loadSif(dataStore.getSif(i), index);
			}
			visualise(index);
			networks[index].setDeselectedSamples(
			    dataStore.getDeSelectedData(index));
			loadSamples(dataStore.getData(i),
			            Discretiser::loadControlFile(
			                dataStore.getDiscretiseControl(i).toStdString()),
			            index);
			networks[index].setQMA(dataStore.getQma(i));
		}
		ui->Output->addItem("Session loaded");
		ui->Output->scrollToBottom();
		if(networks.back().getNumberOfQueries() > 0) {
			ui->actionCreate_Batchfile->setEnabled(true);
			ui->loadPreviousQueryButton->setEnabled(true);
			ui->loadPreviousQueryButton_2->setEnabled(true);
			loadQueriesToHistoryWindow(networks[index]);
		}
	} else {
		ui->Output->addItem("No file specified. No session loaded");
	}
}

void MainWindow::loadQueriesToHistoryWindow(const NetworkInstance& net)
{
	ui->queryHistory->clear();
	for(unsigned int i = 0; i < net.getNumberOfQueries(); ++i) {
		ui->queryHistory->addItem(net.getQuery(i));
	}
}

void MainWindow::on_actionCreate_Batchfile_triggered()
{
	QFileDialog dialog;
	dialog.setDefaultSuffix("txt");
	dialog.exec();
	QString filename = dialog.selectedFiles()[0];
	int index = ui->tabWidget->currentIndex();
	dataStorage dataStore;
	dataStore.createQueryBatchFile(networks[index], filename);
	ui->Output->addItem("Batch file created");
	ui->Output->scrollToBottom();
}

void MainWindow::on_actionExecute_Batchfile_triggered()
{
	std::ifstream input;
	QString filename = QFileDialog::getOpenFileName(
	    this, tr("Load batch file"), config_->dataDir(), "*.txt");
	if(filename == "") {
		ui->Output->addItem("No file specified");
		ui->Output->scrollToBottom();
		return;
	}

	input.open(filename.toStdString());
	ui->Output->addItem("Evaluating queries in: " + filename);
	std::string line;
	while(std::getline(input, line)) {
		if(line == "") {
			continue;
		}

		std::pair<float, std::vector<std::string>> result;
		try {
			result = networks[ui->tabWidget->currentIndex()].calculate(line);
		} catch(std::exception& e) {
			ui->Output->addItem(e.what());
			break;
		}

		ui->Output->addItem(QString::fromStdString(line) + ": " +
		                    QString::number(result.first));

		QString temp = "";
		for(auto& value : result.second) {
			temp += QString::fromStdString(value) + " ";
		}

		if(temp != "") {
			ui->Output->addItem(temp);
		}
	}
}

void MainWindow::on_queryHistory_doubleClicked(const QModelIndex& index)
{
	ui->Input->clear();
	NetworkInstance& currentNetwork = networks[ui->tabWidget->currentIndex()];
	currentNetwork.removeHighlighting();
	currentNetwork.restoreOriginalNetworkRepresentation();
	ui->Input->setText(currentNetwork.getQuery(index.row()));
	writeListWidget(ui->queryVariableList, ui->queryLabel,
	                currentNetwork.getQueryItems(index.row()));
	writeListWidget(ui->conditionVariableList, ui->conditionLabel,
	                currentNetwork.getConditionItems(index.row()));
	writeListWidget(ui->interventionVariableList, ui->interventionLabel,
	                currentNetwork.getInterventionItems(index.row()));
	writeListWidget(ui->edgeAdditionsRemovalList, ui->edgeAdReLabel,
	                currentNetwork.getEdgeAddRemItems(index.row()));
	ui->Input->setFocus();
}
