#include "mainwindow.h"

#include "ui_mainwindow.h"

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

	ui->actionCreate_Batchfile->setEnabled(currentNetwork_()->getQMA().getNumberOfQueries() > 0);
}

void MainWindow::initaliseVisibility()
{
	ui->queryViewDock->hide();
	ui->queryHistoryDock->hide();

	ui->actionSave_Session->setEnabled(false);
}

int MainWindow::generateNetworkInstance()
{
	networks.push_back(new NetworkInstance(this));
	ui->tabWidget->addTab(new QWidget,
	                      "New Tab " + QString::number(ui->tabWidget->count()));
	int index = ui->tabWidget->count() - 1;
	ui->tabWidget->setCurrentIndex(index);
	return index;
}

void MainWindow::loadNAorTGF(QString filename, int index)
{
	addLogMessage("File: " + filename + " opened");
	networks[index]->loadNetwork(filename);
	networks[index]->setNaOrTgf(filename);
	QStringList list = filename.split("/");
	auto name = list[list.size() - 1];
	ui->tabWidget->setTabText(index, name);
}

void MainWindow::loadSif(QString filename, int index)
{
	addLogMessage("File: " + filename + " opened");
	networks[index]->loadNetwork(filename);
	networks[index]->setSif(filename);
}

void MainWindow::visualise(int index)
{
	networks[index]->visualize(ui->tabWidget->widget(index));
	ui->actionLoad_Samples->setEnabled(true);
	ui->actionLayout->setEnabled(true);
	ui->tabWidget->widget(index)->setFocus();
	ui->queryView->setNetworkInstance(networks[index]);

	connect(networks[index], SIGNAL(context(NodeGui*, QContextMenuEvent*)),
	        this, SLOT(Node_context(NodeGui*, QContextMenuEvent*)));
	connect(networks[index], SIGNAL(context(Edge*, QContextMenuEvent*)),
	        this, SLOT(Edge_context(Edge*, QContextMenuEvent*)));
	connect(networks[index], SIGNAL(doubleClick(NodeGui*)),
	        this, SLOT(Node_double_clicked(NodeGui*)));
}

void MainWindow::loadSamples()
{
	unsigned int index =ui->tabWidget->currentIndex(); 
	try {
		networks[index]->setDataFile(networks[index]->getDiscretisationSelection()->samples());
		networks[index]->loadSamples(networks[index]->getDiscretisationSelection()->getPropertyTree());
		adaptQueryEvaluationButtons(true);
		ui->queryView->setNetworkInstance(networks[index]);
		ui->queryView->newQuery();
	} catch(std::invalid_argument& e) {
		addLogMessage(e.what());
		adaptQueryEvaluationButtons(false);
		networks[index]->resetNetwork();
	}
}

void MainWindow::loadSamples(const QString& samples,
				const QString& control,
				unsigned int index)
{
	addLogMessage("Reading samples: " + samples);
	networks[index]->setDataFile(samples);
	networks[index]->setDiscretisationControlFile(control);
	networks[index]->loadSamples();
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
}

void MainWindow::discretiseSelection(const QString& samples,
                                     const std::vector<uint>& deselected)
{
	int index = ui->tabWidget->currentIndex();
	networks[index]->setDeselectedSamples(deselected);
	networks[index]->getDiscretisationSelection()->show(config_->dataDir(), samples, index);
}

void MainWindow::dataRejected()
{
	addLogMessage("No samples loaded");
	adaptQueryEvaluationButtons(false);
	int index = ui->tabWidget->currentIndex();
	networks[index]->resetNetwork();
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

void MainWindow::on_actionLoadNetwork_triggered()
{
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
			addLogMessage(e.what());
		}
	} else {
		addLogMessage("No file containing network data specified.");
	}
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
	checkQueriesLeft();

	if(!networks.empty()) {
		NetworkInstance* currentNetwork = networks[index];
		if(currentNetwork->isTrained()) {
			adaptQueryEvaluationButtons(true);
			currentNetwork->restoreOriginalNetworkRepresentation();
			loadQueriesToHistoryWindow(currentNetwork);
		} else {
			adaptQueryEvaluationButtons(false);
		}

		ui->queryView->setNetworkInstance(currentNetwork);
	} else {
		ui->actionLoad_Samples->setEnabled(false);
		ui->actionLayout->setEnabled(false);
		ui->actionSave_Session->setEnabled(false);
		ui->actionLoad_Session->setEnabled(true);
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
	NetworkInstance* currentNetwork = networks[ui->tabWidget->currentIndex()];

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
	NetworkInstance* currentNetwork = networks[ui->tabWidget->currentIndex()];
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
	NetworkInstance* currentNetwork = networks[ui->tabWidget->currentIndex()];
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
}

void MainWindow::context_Menu_ArgMax_Selected()
{
	ui->queryView->computeArgMax(networks[ui->tabWidget->currentIndex()]->getSelectedNodeID());
}

void MainWindow::context_Menu_QueryValue_Selected(QAction* act)
{
	ui->queryView->computeProbability(networks[ui->tabWidget->currentIndex()]->getSelectedNodeID(), act->text());
}

void MainWindow::context_Menu_ShowMatrix_selected()
{
	NetworkInstance* currentNetwork = networks[ui->tabWidget->currentIndex()];

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
	dataStorage dataStore;
	QFileDialog dialog;
	dialog.setDefaultSuffix("cts");
	dialog.exec();
	QString filename = dialog.selectedFiles()[0];
	if(!filename.isNull()) {
		dataStore.saveSession(networks, filename);
		addLogMessage("Session saved");
	} else {
		addLogMessage("No file specified. The Session was not stored!");
	}
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
			networks[index]->setDeselectedSamples(
			    dataStore.getDeSelectedData(index));
			loadSamples(dataStore.getData(i),
			            dataStore.getDiscretiseControl(i),
			            index);
			networks[index]->setQMA(dataStore.getQma(i));
		}
		addLogMessage("Session loaded");
		if(networks.back()->getQMA().getNumberOfQueries() > 0) {
			ui->actionCreate_Batchfile->setEnabled(true);
			loadQueriesToHistoryWindow(networks[index]);
		}
	} else {
		addLogMessage("No file specified. No session loaded");
	}
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
	int index = ui->tabWidget->currentIndex();
	dataStorage dataStore;
	dataStore.createQueryBatchFile(*networks[index], filename);
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
			result = networks[ui->tabWidget->currentIndex()]->calculate(line);
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
	QString filename = QFileDialog::getSaveFileName(this, tr("Select SVG file."), config_->dataDir(), "*.svg");

	if(filename != "") {
		networks[ui->tabWidget->currentIndex()]->exportSvg(filename);
	}
}

void MainWindow::on_queryHistory_doubleClicked(const QModelIndex& index) {
	if(index.isValid()) {
		//TODO: update the visualization
		ui->queryView->updateQuery(index.row());
	}
}

NetworkInstance* MainWindow::currentNetwork_()
{
	if(ui->tabWidget->currentIndex() == -1) {
		return nullptr;
	}

	return networks[ui->tabWidget->currentIndex()];
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

