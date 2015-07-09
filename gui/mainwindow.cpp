#include "mainwindow.h"

#include "ui_mainwindow.h"

#include "gui_config.h"
#include "Config.h"
#include "datastorage.h"
#include "DataView.h"
#include "DiscretisationSelection.h"
#include "edge.h"
#include "listwidgetmulticopy.h"
#include "matrixpopup.h"
#include "NodeGui.h"

#include "../core/Parser.h"

#include <QtCore/QDir>
#include <QtGui/QDesktopServices>
#include <QtGui/QContextMenuEvent>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>

MainWindow::MainWindow(Config* config, QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), config_(config)
{

	ui->setupUi(this);
	showMaximized();
	initaliseVisibility();

	connect(ui->queryView, SIGNAL(queryExecuted(uint)),
	        this, SLOT(queryExecuted(uint)));
	connect(ui->queryView, SIGNAL(newLogMessage(QString)),
			this, SLOT(addLogMessage(QString)));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionHelp_triggered()
{
	QDesktopServices::openUrl(QUrl("https://github.com/dstoeckel/causaltrail"));
}

void MainWindow::checkQueriesLeft()
{
	ui->actionCreate_Batchfile->setEnabled(
	    !currentNetwork_()->queryHistoryIsEmpty());
}

void MainWindow::initaliseVisibility()
{
	ui->queryViewDock->hide();
	ui->queryHistoryDock->hide();

	ui->actionSave_Session->setEnabled(false);
}

int MainWindow::addNetwork(NetworkInstance* network)
{
	int index = ui->tabWidget->addTab(
	    network, "New Tab " + QString::number(ui->tabWidget->count()));
	ui->tabWidget->setCurrentIndex(index);

	connect(network, &NetworkInstance::newLogMessage, this,
	        &MainWindow::addLogMessage);
	connect(network, &NetworkInstance::samplesLoaded, this,
	        &MainWindow::samplesLoaded);

	return index;
}

int MainWindow::generateNetworkInstance()
{
return addNetwork(new NetworkInstance(this));
}

void MainWindow::loadNAorTGF(QString filename, int index)
{
addLogMessage("File: " + filename + " opened");
getNetwork_(index)->loadNetwork(filename);
getNetwork_(index)->setNaOrTgf(filename);
ui->tabWidget->setTabText(index, filename.split("/").back());
}

void MainWindow::loadSif(QString filename, int index)
{
addLogMessage("File: " + filename + " opened");
getNetwork_(index)->loadNetwork(filename);
getNetwork_(index)->setSif(filename);
}

void MainWindow::visualise(int index)
{
getNetwork_(index)->visualize();
ui->actionLoad_Samples->setEnabled(true);
ui->actionLayout->setEnabled(true);
ui->tabWidget->widget(index)->setFocus();
ui->queryView->setNetworkInstance(getNetwork_(index));

connect(getNetwork_(index), SIGNAL(context(NodeGui*, QContextMenuEvent*)),
	this, SLOT(Node_context(NodeGui*, QContextMenuEvent*)));
connect(getNetwork_(index), SIGNAL(context(Edge*, QContextMenuEvent*)),
	this, SLOT(Edge_context(Edge*, QContextMenuEvent*)));
connect(getNetwork_(index), SIGNAL(doubleClick(NodeGui*)), this,
	SLOT(Node_double_clicked(NodeGui*)));
}

void MainWindow::samplesLoaded(NetworkInstance* network)
{
addLogMessage("Network trained");
adaptQueryEvaluationButtons(true);
ui->queryView->setNetworkInstance(network);
ui->queryView->newQuery();
}

void MainWindow::loadSamples(const QString& samples, const QString& control,
		     unsigned int index)
{
addLogMessage("Reading samples: " + samples);
getNetwork_(index)->loadSamples(samples, control);
adaptQueryEvaluationButtons(true);

ui->queryView->newQuery();
}

void MainWindow::addLogMessage(const QString& msg) {
ui->Output->addItem(msg);
ui->Output->scrollToBottom();
}

void MainWindow::queryExecuted(unsigned int query)
{
	loadQueriesToHistoryWindow(currentNetwork_());
	checkQueriesLeft();
}

void MainWindow::discretiseSelection(const QString& samples,
			     const std::vector<uint>& deselected)
{
currentNetwork_()->discretise(samples, deselected);
}

void MainWindow::dataRejected()
{
addLogMessage("No samples loaded");
adaptQueryEvaluationButtons(false);
currentNetwork_()->resetNetwork();
}

void MainWindow::on_actionLoad_Samples_triggered()
{
try {
	QString samples = QFileDialog::getOpenFileName(
	    this, tr("Open txt file containing samples"), config_->dataDir(),
	    "*.txt");

	if(samples.isNull()) {
		throw std::invalid_argument(
		    "No file containing samples specified!");
	}

	DataView* dView = new DataView(this, samples);
	connect(dView, &DataView::dataAccepted, this,
		&MainWindow::discretiseSelection);
	connect(dView, &DataView::rejected, this, &MainWindow::dataRejected);
	dView->show();
} catch(std::exception& e) {
	addLogMessage(e.what());
	adaptQueryEvaluationButtons(false);
	currentNetwork_()->resetNetwork();
}
}

void MainWindow::on_actionAbout_triggered()
{
QMessageBox box;
box.setIcon(QMessageBox::Information);
box.setWindowTitle(tr("About CausalTrail"));
// Hack around the line wrap for the copyright statement.
box.setText(tr("CausalTrail") + "                                                          ");
box.setInformativeText(tr(
	"A statistical tool to perform inference in causal "
	"bayesian networks using Do-Calculus.\n\n"
	"Copyright 2014-2015, Florian Schmidt\n"
	"Copyright 2014-2015, Daniel Stöckel"
));

box.exec();
}


void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
ui->tabWidget->removeTab(index);
}

void MainWindow::on_actionDeleteNetwork_triggered()
{
ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
}

void MainWindow::on_actionLoadNetwork_triggered()
{
	QString filename = QFileDialog::getOpenFileName(
	    this, tr("Load network file"), config_->dataDir(), "*.tgf *.na");

	if(filename == "") {
		addLogMessage("No file containing network data specified.");
		return;
	}

	int index = generateNetworkInstance();
	try {
		loadNAorTGF(filename, index);
		if(filename.endsWith(".na")) {
			filename = QFileDialog::getOpenFileName(
			    this, tr("Load sif file"), QFileInfo(filename).absolutePath(),
			    "*.sif");
			loadSif(filename, index);
		}

		visualise(index);
	} catch(std::exception& e) {
		addLogMessage(e.what());
		ui->tabWidget->removeTab(index);
	}
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
	bool validInstance = index != -1;

	ui->actionExportSvg->setEnabled(validInstance);
	ui->actionLoad_Samples->setEnabled(validInstance);
	ui->actionLayout->setEnabled(validInstance);
	ui->actionSave_Session->setEnabled(validInstance);
	ui->actionDeleteNetwork->setEnabled(validInstance);

	if(validInstance) {

		checkQueriesLeft();
		NetworkInstance* currentNetwork = getNetwork_(index);
		if(currentNetwork->isTrained()) {
			adaptQueryEvaluationButtons(true);
			currentNetwork->restoreOriginalNetworkRepresentation();
			loadQueriesToHistoryWindow(currentNetwork);
	            if (!currentNetwork->queryHistoryIsEmpty()){
        	        ui->actionCreate_Batchfile->setEnabled(true);
           		}
		} else {
			adaptQueryEvaluationButtons(false);
           		 ui->actionCreate_Batchfile->setEnabled(false);
			}
		ui->queryView->setNetworkInstance(currentNetwork);
	} else {
		ui->actionExecute_Batchfile->setEnabled(false);
		ui->actionCreate_Batchfile->setEnabled(false);
		ui->queryView->setNetworkInstance(nullptr);
	}
}

void MainWindow::on_actionLayout_triggered()
{
currentNetwork_()->layout();
}

void MainWindow::Node_double_clicked(NodeGui* node)
{
NetworkInstance* currentNetwork = currentNetwork_();

currentNetwork->setSelectedNode(node->getID());
if(!currentNetwork->edgeAddition()) {
	return;
}

if(currentNetwork->checkEdgeAddition(node->getID())) {
	currentNetwork->nodeForEdgeAdditionSelected(node->getID());
} else {
	addLogMessage("This edge would induce a cycle. Therefore, it "
		      "can not be added!");
}
}

void MainWindow::Node_context(NodeGui* node,
		      QContextMenuEvent* event)
{
NetworkInstance* currentNetwork = currentNetwork_();
QMenu* menu = new QMenu;
if(currentNetwork->isTrained()) {
	const auto& values = currentNetwork->getValues(node->getID());
	currentNetwork->setSelectedNode(node->getID());
	QMenu* submenu;
	// Non-Intervention Query
	if(ui->queryView->numQueryVariables() == 0 || !currentNetwork->isArgMax()) {
		submenu = menu->addMenu("Probability of");
		for(auto& value : values) {
			submenu->addAction(QString::fromStdString(value));
			connect(submenu, SIGNAL(triggered(QAction*)), this,
				SLOT(context_Menu_QueryValue_Selected(QAction*)),
				Qt::UniqueConnection);
		}
	}

	// Arg-Max
	if((ui->queryView->numQueryVariables() == 0) || currentNetwork->isArgMax()) {
		connect(menu->addAction("Arg Max"), SIGNAL(triggered()), this,
			SLOT(context_Menu_ArgMax_Selected()), Qt::UniqueConnection);
	}

	// Condition
	submenu = menu->addMenu("Condition on");
	for(auto& value : values) {
		submenu->addAction(QString::fromStdString(value));
		connect(submenu, SIGNAL(triggered(QAction*)), this,
			SLOT(conditionValueSelected(QAction*)),
			Qt::UniqueConnection);
	}

	menu->addSeparator();

	// Do-Int
	submenu = menu->addMenu("Set value to");
	for(auto& value : values) {
		submenu->addAction(QString::fromStdString(value));
		connect(submenu, SIGNAL(triggered(QAction*)), this,
			SLOT(interventionValueSelected(QAction*)),
			Qt::UniqueConnection);
	}
	// Add Edge To
	connect(menu->addAction("Add Edge to ..."), SIGNAL(triggered()), this,
		SLOT(addEdgeSelected()), Qt::UniqueConnection);

	menu->addSeparator();

	// Show Probability Matrix
	connect(menu->addAction("Show CPT"), SIGNAL(triggered()), this,
		SLOT(context_Menu_ShowMatrix_selected()), Qt::UniqueConnection);

} else {
	menu->addAction("The network is not trained");
}
menu->popup(event->globalPos());
}

void MainWindow::Edge_context(Edge* edge, QContextMenuEvent* event)
{
NetworkInstance* currentNetwork = currentNetwork_();
	currentNetwork->addEdgeRemoval(edge->sourceNode()->getID(), edge->destNode()->getID());
	QMenu* menu = new QMenu;
	if(currentNetwork->isTrained()) {
		connect(menu->addAction("Remove Edge"), SIGNAL(triggered()), this,
		        SLOT(removeEdgeSelected()), Qt::UniqueConnection);
	} else {
		menu->addAction("The network is not trained");
	}
	menu->popup(event->globalPos());
}

void MainWindow::adaptQueryEvaluationButtons(bool show)
{
	ui->queryViewDock->setVisible(show);
	ui->queryHistoryDock->setVisible(show);
	ui->actionSave_Session->setEnabled(show);
	ui->actionExecute_Batchfile->setEnabled(show);
    ui->actionSave_discretised_data->setEnabled(show);
}

void MainWindow::context_Menu_ArgMax_Selected()
{
	ui->queryView->computeArgMax(currentNetwork_()->getSelectedNodeID());
}

void MainWindow::context_Menu_QueryValue_Selected(QAction* act)
{
	ui->queryView->computeProbability(currentNetwork_()->getSelectedNodeID(), act->text());
}

void MainWindow::context_Menu_ShowMatrix_selected()
{
	NetworkInstance* currentNetwork = currentNetwork_();

	matrixPopup* mp = new matrixPopup();
	mp->setWindowTitle("CPT of " + currentNetwork->getSelectedNodeName());

	const unsigned int rowCount =
	    currentNetwork->getRowCountOfCurrentProbMatrix();
	const unsigned int colCount =
	    currentNetwork->getColCountOfCurrentProbMatrix();
	const unsigned int parentCount =
	    currentNetwork->getNumberOfParentsOfSelectedNode();
	mp->initaliseTable(rowCount + 1, colCount + parentCount);

	mp->setRowNames(currentNetwork->getRowNamesOfProbabilityMatrix());
	mp->setColNames(currentNetwork->getColNamesOfProbabilityMatrix(),
	                parentCount);
	mp->setParentNames(currentNetwork->getParentsOfSelectedNode());

	for(unsigned int row = 0; row < rowCount; row++) {
		for(unsigned int col = 0; col < colCount; col++) {
			mp->addData(row + 1, col + parentCount,
			            currentNetwork->getProbabilityAt(row, col));
		}
	}
	mp->adjust();
	mp->show();
	currentNetwork->removeHighlighting();
}

void MainWindow::on_actionSave_Session_triggered()
{
	QString filename =
	    QFileDialog::getSaveFileName(this, tr("Save session"), "", "cts");
	if(filename.isNull()) {
		addLogMessage("No file specified. The Session was not stored!");
		return;
	}

	std::vector<NetworkInstance*> networks(ui->tabWidget->count());
	for(int i = 0; i < ui->tabWidget->count(); ++i) {
		networks[i] = getNetwork_(i);
	}

	dataStorage dataStore;
	dataStore.saveSession(networks, filename);
	addLogMessage("Session saved");
}

void MainWindow::on_actionLoad_Session_triggered()
{
	dataStorage dataStore;
	QString filename = QFileDialog::getOpenFileName(
	    this, tr("Load session file"), config_->dataDir(), "*.cts");
	if(filename == "") {
		addLogMessage("No file specified. No session loaded");
		return;
	}

	for(auto* net : dataStore.loadSession(filename)) {
		net->loadNetwork();
		net->loadSamples(net->getDataFile(), net->getDiscretisationSettings());
		int index = addNetwork(net);
		visualise(index);
		ui->tabWidget->setTabText(index, filename.split("/").back());
	}

	addLogMessage("Session loaded");
}

void MainWindow::loadQueriesToHistoryWindow(const NetworkInstance* net)
{
	ui->queryHistory->clear();

	for(auto&& query : net->getQMA().getQueries()) {
		ui->queryHistory->addItem(query);
	}
}

void MainWindow::on_actionCreate_Batchfile_triggered()
{
	QFileDialog dialog;
	dialog.setDefaultSuffix("txt");
	dialog.exec();
	QString filename = dialog.selectedFiles()[0];
	dataStorage dataStore;
	dataStore.createQueryBatchFile(*currentNetwork_(), filename);
	addLogMessage("Batch file created");
}

void MainWindow::on_actionExecute_Batchfile_triggered()
{
	std::ifstream input;
	QString filename = QFileDialog::getOpenFileName(
	    this, tr("Load batch file"), config_->dataDir(), "*.txt");
	if(filename == "") {
		addLogMessage("No file specified");
		return;
	}

	input.open(filename.toStdString());
	addLogMessage("Evaluating queries in: " + filename);
	std::string line;
	while(std::getline(input, line)) {
		if(line == "") {
			continue;
		}

		std::pair<float, std::vector<std::string>> result;
		try {
			result = currentNetwork_()->calculate(line);
		} catch(std::exception& e) {
			addLogMessage(e.what());
			break;
		}

		addLogMessage(QString::fromStdString(line) + ": " +
		              QString::number(result.first));

		QString temp = "";
		for(auto& value : result.second) {
			temp += QString::fromStdString(value) + " ";
		}

		if(temp != "") {
			addLogMessage(temp);
		}
	}
}

void MainWindow::on_actionExportSvg_triggered()
{
#ifdef CT_HAS_QT5_SVG
	QString filename = QFileDialog::getSaveFileName(this, tr("Select SVG file."), config_->dataDir(), "*.svg");

	if(filename != "") {
		currentNetwork_()->exportSvg(filename);
	}
#else
	QMessageBox::critical(
	    this, tr("No SVG support available"),
	    tr("This version of CausalTrail has not been compiled with SVG "
	       "support. This means that no export to SVG is possible."));
#endif
}

void MainWindow::on_queryHistory_doubleClicked(const QModelIndex& index) {
	if(index.isValid()) {
		ui->queryView->updateQuery(index.row());
	}
}

NetworkInstance* MainWindow::currentNetwork_()
{
	return static_cast<NetworkInstance*>(ui->tabWidget->currentWidget());
}

NetworkInstance* MainWindow::getNetwork_(int index)
{
	return static_cast<NetworkInstance*>(ui->tabWidget->widget(index));
}

void MainWindow::addEdgeSelected()
{
	NetworkInstance* currentNetwork = currentNetwork_();
	currentNetwork->initialiseEdgeAddition();
	currentNetwork->nodeForEdgeAdditionSelected(currentNetwork->getSelectedNodeID());
	currentNetwork->removeHighlighting();
}

void MainWindow::removeEdgeSelected() {
	currentNetwork_()->removeSelectedEdge();
}

void MainWindow::conditionValueSelected(QAction* action)
{
	ui->queryView->conditionOn(currentNetwork_()->getSelectedNodeID(), action->text());
}

void MainWindow::interventionValueSelected(QAction* action)
{
	ui->queryView->interventionOn(currentNetwork_()->getSelectedNodeID(), action->text());
}


void MainWindow::on_actionSave_discretised_data_triggered()
{
    NetworkInstance* currentNetwork = currentNetwork_();
    QString filename = QFileDialog::getSaveFileName(this, tr("Select txt file."), config_->dataDir(), "*.txt");
    if(filename != "") {
        currentNetwork->saveDiscretisedData(filename);
        addLogMessage("Discretised data written to "+filename);
    }
    else
    {
       addLogMessage("No file specified, discretised data could not be stored.");
    }
}
