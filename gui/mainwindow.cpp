#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "matrixpopup.h"
#include "ui_matrixpopup.h"
#include "dataview.h"
#include "ui_dataview.h"
#include "discretisationselection.h"
#include "ui_discretisationselection.h"
#include "QFileDialog"
#include "QGraphicsSceneContextMenuEvent"
#include "QMessageBox"
#include "../core/Parser.h"
#include "NodeGui.h"
#include "edge.h"
#include "datastorage.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadConfigFile();
    showMaximized();
    initaliseVisibility();
}

void MainWindow::loadConfigFile(){
    std::ifstream configfile;
    configfile.open("CausalTrailPathConfig.txt");
    if (configfile){
        ui->Output->addItem("Found CausalTrailPathConfig.txt, path  loaded");
        std::string ts;
        configfile >> ts;
        path = QString::fromStdString(ts);
        configfile.close();
    }
    else {
        std::ofstream newConfigfile;
        newConfigfile.open("CausalTrailPathConfig.txt");
        QString pathtemp = QFileDialog::getExistingDirectory(0,"Specify the folder containing your data",QString(),QFileDialog::ShowDirsOnly);
        path = pathtemp;
        newConfigfile<<pathtemp.toStdString();
        newConfigfile.close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionClose_triggered()
{
    close();
}

void MainWindow::checkQueriesLeft(){
	if (networks.size() > 0){
 	   int index = ui->tabWidget->currentIndex();
 	   if (networks[index].getNumberOfQueries() > 0){
 	       ui->actionCreate_Batchfile->setEnabled(true);
 	       ui->actionCreate_Batchfile_2->setEnabled(true);
 	   }
 	   else{
 	       ui->actionCreate_Batchfile->setEnabled(false);
	       ui->actionCreate_Batchfile_2->setEnabled(false);
	   }
	   if (networks[index].isFirstQuery()){
    	   ui->loadPreviousQueryButton->setEnabled(false);
	       ui->loadPreviousQueryButton_2->setEnabled(false);
	   }
	   else{
	       ui->loadPreviousQueryButton->setEnabled(true);
	       ui->loadPreviousQueryButton_2->setEnabled(true);
	   }
	   if (networks[index].isLastQuery()){
	       ui->loadSuccessorQueryButton->setEnabled(false);
	       ui->loadSuccessorQueryButton_2->setEnabled(false);
	   }
	   else{
	       ui->loadSuccessorQueryButton->setEnabled(true);
	       ui->loadSuccessorQueryButton_2->setEnabled(true);
	   }
	}
}

void MainWindow::initaliseVisibility(){
    ui->dockWidget->setVisible(false);
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
    ui->actionCreate_Batchfile_2->setEnabled(false);
    ui->actionExecute_Batchfile_2->setEnabled(false);
    ui->actionSave_Session->setEnabled(false);
    ui->actionLoad_Samples_2->setEnabled(false  );
}

void MainWindow::checkAllEmpty()
{
	if((ui->queryVariableList->count() == 0)
		&&(ui->conditionVariableList->count() == 0) &&
		    (ui->interventionVariableList->count() == 0) &&
		    (ui->edgeAdditionsRemovalList->count() == 0))
		{
			on_newQuery_clicked();
		}
	ui->Input->setFocus();
}

void MainWindow::checkEmptyList(QListWidget *widget, QLabel *label)
{
    if (widget->count() == 0){
        widget->setVisible(false);
        label->setVisible(false);
    }
}

int MainWindow::removeDuplicates(QListWidget *widget, QString nodeName, unsigned int index)
{
    for (int i = 0; i< widget->count(); i++){
        if (nodeName == widget->item(i)->text().split(" ")[index]){
            return i;
        }
    }
    return -1;
}

std::vector<QString> MainWindow::getVector(QListWidget *widget)
{
    std::vector<QString> temp;
    for (int i = 0; i< widget->count(); i++){
        temp.push_back(widget->item(i)->text());
    }
    return temp;
}

void MainWindow::writeListWidget(QListWidget *widget, QLabel* label, std::vector<QString>& vec){
    widget->clear();
    if (vec.empty()){
        widget->setVisible(false);
        label->setVisible(false);
    }
    else{
        widget->setVisible(true);
        label->setVisible(true);
    }
    for (auto item : vec){
        widget->addItem(item);
    }
}

int MainWindow::generateNetworkInstance(){
    networks.push_back(NetworkInstance());
    ui->dockWidget->setVisible(true);
    ui->tabWidget->addTab(new QWidget,"New Tab "+QString::number(ui->tabWidget->count()));
    int index = ui->tabWidget->count()-1;
    ui->tabWidget->setCurrentIndex(index);
    return index;
}

void MainWindow::loadNAorTGF(QString filename, int index){
    ui->Output->addItem("File: "+filename+ " opened");
    networks[index].loadNetwork(filename);
    networks[index].setNaOrTgf(filename);
    QStringList list = filename.split("/");
    auto name = list[list.size()-1];
    ui->tabWidget->setTabText(index,name);
}

void MainWindow::loadSif(QString filename, int index){
   ui->Output->addItem("File: "+filename+ " opened");
   networks[index].loadNetwork(filename);
   networks[index].setSif(filename);
}

void MainWindow::visualise(int index){
    networks[index].visualize(ui->tabWidget->widget(index));
    ui->actionLoad_Samples->setEnabled(true);
    ui->actionLayout->setEnabled(true);
    ui->tabWidget->widget(index)->setFocus();
    for (const NodeGui* ng: networks[index].getNodeGuiVec()){
        connect(ng,SIGNAL(doubleClick(uint,QString)),this,SLOT(Node_double_clicked(uint,QString)));
        connect(ng,SIGNAL(context(QString, uint,QGraphicsSceneContextMenuEvent*)),this,SLOT(Node_context(QString, uint,QGraphicsSceneContextMenuEvent*)));
    }
    for (const Edge* edge : networks[index].getEdgeVec()){
        connect(edge,SIGNAL(context(QString, QString, unsigned int, unsigned int, QGraphicsSceneContextMenuEvent*)), this, SLOT(Edge_context(QString, QString, unsigned int, unsigned int, QGraphicsSceneContextMenuEvent*)));
    }
    networks[index].setWidth(MainWindow::width()-ui->dockWidget_5->width());
    networks[index].resizeNV(networks[index].getNVSizeHint());
}

void MainWindow::loadSamples(QString samples, QString controlDiscret, int index){
    ui->Output->addItem("Reading samples: "+samples);
    ui->Output->addItem("Discretising according to: "+controlDiscret);
    networks[index].loadSamples(samples,controlDiscret);
    networks[index].setDiscretisationControlFile(controlDiscret);
    networks[index].setDataFile(samples);
    adaptQueryEvaluationButtons(true);
    on_newQuery_clicked();
}

void MainWindow::discretisationFileCreated(QString control, QString samples, int index){
    try{
        loadSamples(samples,control,index);
    }
    catch (std::exception& e){
        ui->Output->addItem(e.what());
        adaptQueryEvaluationButtons(false);
        networks[index].resetNetwork();
    }
    ui->Output->scrollToBottom();
}

void MainWindow::discretiseSelection(QString samples, std::vector<uint> deselected){
    int index = ui->tabWidget->currentIndex();
    networks[index].setDeselectedSamples(deselected);
    QString control;
    QMessageBox boxDisc;
    boxDisc.setIcon(QMessageBox::Question);
    boxDisc.setText("Please specify discretisation information source.");
    QPushButton* file = boxDisc.addButton("Load from File",QMessageBox::ActionRole);
    QPushButton* choose = boxDisc.addButton("Choose Methods", QMessageBox::ActionRole);
    boxDisc.exec();
	try{
	    if (boxDisc.clickedButton()==file){
	         control = QFileDialog::getOpenFileName(this, tr("Open txt file containing discretisation information"),path,"*.txt");
	         loadSamples(samples,control,index);
	    }
	    else{
	        if (boxDisc.clickedButton()==choose){
	            discretisationSelection* discSel = new discretisationSelection(0,path,samples,index);
	            connect(discSel,SIGNAL(fileSaved(QString,QString,int)),this,SLOT(discretisationFileCreated(QString,QString,int)));
    	        discSel->adaptGUIToData();
	            discSel->show();
	        }
	        else{
	            throw std::invalid_argument("Discretisation information is not specified");
	        }
	    }
	}
	catch(std::exception& e){
		ui->Output->addItem(e.what());
		adaptQueryEvaluationButtons(false);
		networks[index].resetNetwork();
	}
	ui->Output->scrollToBottom();

}

void MainWindow::dataRejected(){
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
        QString samples = QFileDialog::getOpenFileName(this, tr("Open txt file containing samples"),path,"*.txt");
        QMessageBox boxSamples;
        boxSamples.setIcon(QMessageBox::Question);
        boxSamples.setText("Do you want to view/edit the data?");
        QPushButton* yes = boxSamples.addButton("Yes", QMessageBox::ActionRole);
        boxSamples.addButton("No",QMessageBox::ActionRole);
        boxSamples.exec();
        if (boxSamples.clickedButton()==yes){
            //View -> Call a new window
             dataview* dView = new dataview(0,samples);
             dView->setWindowTitle("Data contained in "+samples);
             connect(dView,SIGNAL(dataSubmitted(QString, std::vector<uint>)),this,SLOT(discretiseSelection(QString,std::vector<uint>)));
             connect(dView,SIGNAL(dataRejected()),this,SLOT(dataRejected()));
             dView->adjust();
             dView->update();
             dView->show();
        }
        else{
            std::vector<unsigned int> temp;
            discretiseSelection(samples,temp);
        }
    }
    catch (std::exception& e){
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
    box.setInformativeText("A statistical tool to performe inference in causal bayesian networks using Do-Calculus.");
    box.exec();
}

void MainWindow::on_executeQueryButton_clicked()
{
    QString query_=buildQuery();
    if (query_!= "?"){
        ui->Input->setText(query_);
    }
    int index = ui->tabWidget->currentIndex();
    std::pair<float,std::vector<std::string>> result;
    try{
        result = networks[index].calculate(ui->Input->text());
        ui->Output->addItem(ui->Input->text()+": "+QString::number(result.first));
        ui->probLabel->setVisible(true);
        ui->probResultLabel->setText(QString::number(result.first));
        QString temp="";
        for (unsigned int i = 0; i< result.second.size(); i++){
            temp+=ui->queryVariableList->item(i)->text().split(" ")[1]+"="+QString::fromStdString(result.second[i])+" ";
        }
        if (temp != ""){
            ui->valueLabel->setVisible(true);
            ui->Output->addItem(temp);
            ui->valueMaxLabel->setText(temp);
         }
        networks[index].storeQuery(ui->Input->text(),getVector(ui->queryVariableList),getVector(ui->conditionVariableList),getVector(ui->interventionVariableList),getVector(ui->edgeAdditionsRemovalList));
        loadQueriesToHistoryWindow(index);
        checkQueriesLeft();
        query_="";
    }
    catch(std::exception& e){
        ui->Output->addItem(e.what());
    }
    networks[index].removeHighlighting();
    ui->Output->scrollToBottom();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    networks.erase(networks.begin()+index);
	ui->tabWidget->removeTab(index);
}

void MainWindow::on_actionDeleteNetwork_triggered()
{
    if (networks.size()>0){
        int index = ui->tabWidget->currentIndex();
		networks.erase(networks.begin()+index);
        ui->tabWidget->removeTab(index);
    }
}

void MainWindow::on_actionNewNetwork_triggered()
{
    if (ui->deleteQueryButton->isEnabled()){
        on_deleteQueryButton_clicked();
    }
    QString filename = QFileDialog::getOpenFileName(this,tr("Load network file"),path,tr("*tgf *na"));
    if (filename != ""){
        try {
            int index = generateNetworkInstance();
            loadNAorTGF(filename,index);
            if (boost::filesystem::extension(filename.toStdString())==".na"){
                filename = QFileDialog::getOpenFileName(this,tr("Load sif file"),path,tr("*sif"));
                loadSif(filename,index);
            }
            visualise(index);

        }
        catch (std::exception& e){
            ui->Output->addItem(e.what());
        }
    }
    ui->Output->scrollToBottom();
}

void MainWindow::on_deleteQueryButton_clicked(){
    ui->probLabel->setVisible(false);
    ui->valueLabel->setVisible(false);
    ui->probResultLabel->clear();
    ui->valueMaxLabel->clear();
}

void MainWindow::on_loadPreviousQueryButton_clicked(){
    on_deleteQueryButton_clicked();
    int index = ui->tabWidget->currentIndex();
    networks[index].removeHighlighting();
    networks[index].restoreOriginalNetworkRepresentation();
    ui->Input->setText(networks[index].getPreviousQuery());
    writeListWidget(ui->queryVariableList,ui->queryLabel,networks[index].getPreviousQueryItems());
    writeListWidget(ui->conditionVariableList, ui->conditionLabel, networks[index].getPreviousConditionItems());
    writeListWidget(ui->interventionVariableList, ui->interventionLabel, networks[index].getPreviousInterventionItems());
    writeListWidget(ui->edgeAdditionsRemovalList, ui->edgeAdReLabel, networks[index].getPreviousEdgeChangeItems());
    ui->queryLabel->setText("Calculating probability of");
    checkQueriesLeft();

}

void MainWindow::on_loadSuccessorQueryButton_clicked(){
    on_deleteQueryButton_clicked();
    int index = ui->tabWidget->currentIndex();
    networks[index].removeHighlighting();
    networks[index].restoreOriginalNetworkRepresentation();
    ui->Input->setText(networks[index].getSubsequentQuery());
    writeListWidget(ui->queryVariableList, ui->queryLabel, networks[index].getSubsequentQueryItems());
    writeListWidget(ui->conditionVariableList, ui->conditionLabel, networks[index].getSubsequentConditionItems());
    writeListWidget(ui->interventionVariableList, ui->interventionLabel, networks[index].getSubsequentInterventionItems());
    writeListWidget(ui->edgeAdditionsRemovalList, ui->edgeAdReLabel, networks[index].getSubsequentEdgeChangeItems());
    ui->queryLabel->setText("Calculating probability of");
    checkQueriesLeft();
}


void MainWindow::on_tabWidget_currentChanged(int index){
    checkQueriesLeft();
    if (networks.size() > 0){
        if (networks[index].isTrained()){
            adaptQueryEvaluationButtons(true);
            networks[index].restoreOriginalNetworkRepresentation();
            loadQueriesToHistoryWindow(index);
        }
        else{
            adaptQueryEvaluationButtons(false);
        }
    }
    else{
        ui->actionLoad_Samples->setEnabled(false);
        ui->actionLoad_Samples_2->setEnabled(false);
        ui->actionLayout->setEnabled(false);
        ui->actionLayout_2->setEnabled(false);
        ui->actionSaveSession->setEnabled(false);
        ui->actionSave_Session->setEnabled(false);
        ui->actionLoad_Session->setEnabled(true);
        ui->actionLoad_Session_2->setEnabled(true);
        ui->actionExecute_Batchfile->setEnabled(false);
        ui->actionExecute_Batchfile_2->setEnabled(false);
        ui->actionCreate_Batchfile->setEnabled(false);
        ui->actionCreate_Batchfile_2->setEnabled(false);
    }
    clearLabelsAndValueList();
}


void MainWindow::on_actionLayout_triggered(){
    int index = ui->tabWidget->currentIndex();
    networks[index].layout();
}

void MainWindow::Node_double_clicked(uint id, QString name){
    int index = ui->tabWidget->currentIndex();
    QString sourceName = networks[index].getSelectedNodeName();
    networks[index].setSelectedNode(id,name);
    if (networks[index].EdgeAddition()){
        if(networks[index].checkEdgeAddition(id)){
            ui->edgeAdReLabel->setVisible(true);
            ui->edgeAdditionsRemovalList->setVisible(true);
            networks[index].NodeForEdgeAdditionSelected(id);
            ui->edgeAdditionsRemovalList->addItem("+ "+sourceName+" "+name);
            if (not networks[index].EdgeAddition()){
                ui->Input->setFocus();
            }
        }
        else{
            ui->Output->addItem("This edge would induce a cycle. Therefore, it can not be added!");
            ui->Output->scrollToBottom();
        }
    }
}


void MainWindow::on_newQuery_clicked()
{
    ui->deleteQueryButton->click();
    clearLabelsAndValueList();
    if (networks.empty()==false){
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


void MainWindow::Node_context(QString name, uint id, QGraphicsSceneContextMenuEvent * event)
{
    int index = ui->tabWidget->currentIndex();
    QMenu* menu = new QMenu;
    if (networks[index].isTrained()){
        auto& values = networks[index].getValues(id);
        networks[index].setSelectedNode(id,name);
        QMenu* submenu;
       //Non-Intervention Query
        if ((ui->queryVariableList->count()==0) ||(not networks[index].isArgMax())){
           submenu = menu->addMenu("Probability of");
           for (auto& value : values){
                submenu->addAction(QString::fromStdString(value));
                connect(submenu, SIGNAL(triggered(QAction*)), this, SLOT(context_Menu_QueryValue_Selected(QAction*)), Qt::UniqueConnection);
           }
        }

        //Arg-Max
        if ((ui->queryVariableList->count()==0) || networks[index].isArgMax()){
            connect(menu->addAction("Arg Max"), SIGNAL(triggered()), this, SLOT(context_Menu_ArgMax_Selected()),Qt::UniqueConnection);
        }

        //Condition
        submenu = menu->addMenu("Condition on");
        for (auto& value : values){
            submenu->addAction(QString::fromStdString(value));
            connect(submenu, SIGNAL(triggered(QAction*)), this, SLOT(context_Menu_ConditionValue_Selected(QAction*)), Qt::UniqueConnection);
        }

        menu->addSeparator();

        //Do-Int
        submenu = menu->addMenu("Set value to");
        for (auto& value : values){
            submenu->addAction(QString::fromStdString(value));
            connect(submenu, SIGNAL(triggered(QAction*)), this, SLOT(context_Menu_InterventionValue_Selected(QAction*)), Qt::UniqueConnection);
        }
        //Add Edge To
        connect(menu->addAction("Add Edge to ..."), SIGNAL(triggered()), this, SLOT(context_Menu_AddEdge_selected()),Qt::UniqueConnection);

        menu->addSeparator();

        //Show Probability Matrix
        connect(menu->addAction("Show Probability Matrix"), SIGNAL(triggered()), this, SLOT(context_Menu_ShowMatrix_selected()),Qt::UniqueConnection);

    }
    else{
        menu->addAction("The network is not trained");
    }
    menu->popup(event->screenPos());
    ui->Input->setFocus();
}

void MainWindow::Edge_context(QString sourceName, QString destName, unsigned int sourceID, unsigned int destID, QGraphicsSceneContextMenuEvent* event){
    int index = ui->tabWidget->currentIndex();
    networks[index].EdgeRemoval(sourceID,destID,sourceName,destName);
    QMenu* menu = new QMenu;
    if (networks[index].isTrained()){
        connect(menu->addAction("Remove Edge"),SIGNAL(triggered()),this, SLOT(context_EdgeRemove_Selected()),Qt::UniqueConnection);
    }
    else{
         menu->addAction("The network is not trained");
    }
    menu->popup(event->screenPos());
}

void MainWindow::adaptQueryEvaluationButtons(bool show){
    ui->dockWidget_2->setVisible(show);
    ui->dockWidget_5->setVisible(show);
    ui->dockWidget_3->setVisible(show);
    ui->dockWidgetContents_4->setEnabled(show);
    ui->dockWidgetContents_2->setEnabled(show);
    ui->executeQueryButton_2->setEnabled(show);
    ui->deleteQueryButton_2->setEnabled(show);
    ui->actionSaveSession->setEnabled(show);
    ui->actionSave_Session->setEnabled(show);
    ui->actionExecute_Batchfile->setEnabled(show);
    ui->actionExecute_Batchfile_2->setEnabled(show);
}

void MainWindow::clearLabelsAndValueList(){
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
    int index = ui->tabWidget->currentIndex();
    ui->queryLabel->setText("Calculating probability of");
    int duplicates = removeDuplicates(ui->interventionVariableList, networks[index].getSelectedNodeName(), 0);
    if (duplicates != -1){
        on_interventionVariableList_itemDoubleClicked(ui->interventionVariableList->item(duplicates));
    }
    duplicates = removeDuplicates(ui->queryVariableList,networks[index].getSelectedNodeName(),0);
    if (duplicates != -1){
        on_queryVariableList_itemDoubleClicked(ui->queryVariableList->item(duplicates));
    }
    duplicates = removeDuplicates(ui->queryVariableList,networks[index].getSelectedNodeName(),1);
    if (duplicates != -1){
        on_queryVariableList_itemDoubleClicked(ui->queryVariableList->item(duplicates));
    }
    ui->queryVariableList->addItem(networks[index].getSelectedNodeName()+" = "+act->text());
    ui->queryVariableList->setVisible(true);
    ui->queryLabel->setVisible(true);
    QColor queryColor;
    queryColor.setHsv(100,30,250);
    networks[index].colorNode(networks[index].getSelectedNodeID(),queryColor);
    networks[index].removeHighlighting();
}

void MainWindow::context_Menu_ArgMax_Selected()
{
    int index = ui->tabWidget->currentIndex();
    networks[index].setArgMax(true);
    ui->queryLabel->setText("Calculating the most probable value assignment for");
    int duplicates = removeDuplicates(ui->queryVariableList, networks[index].getSelectedNodeName(),1);
    if (duplicates != -1){
        on_queryVariableList_itemDoubleClicked(ui->queryVariableList->item(duplicates));
    }
    duplicates = removeDuplicates(ui->interventionVariableList, networks[index].getSelectedNodeName(), 0);
    if (duplicates != -1){
        on_interventionVariableList_itemDoubleClicked(ui->interventionVariableList->item(duplicates));
    }
    duplicates = removeDuplicates(ui->conditionVariableList, networks[index].getSelectedNodeName(), 0);
    if (duplicates != -1){
        on_conditionVariableList_itemDoubleClicked(ui->conditionVariableList->item(duplicates));
    }
    ui->queryVariableList->addItem("argmax "+networks[index].getSelectedNodeName());
    ui->queryLabel->setVisible(true);
    ui->queryVariableList->setVisible(true);
    QColor queryColor;
    queryColor.setHsv(100,30,250);
    networks[index].colorNode(networks[index].getSelectedNodeID(),queryColor);
    networks[index].removeHighlighting();
}

void MainWindow::context_Menu_ConditionValue_Selected(QAction* act)
{
    int index = ui->tabWidget->currentIndex();
    ui->conditionLabel->setText("given that");
    int duplicates = removeDuplicates(ui->conditionVariableList, networks[index].getSelectedNodeName(), 0);
    if (duplicates != -1){
        on_conditionVariableList_itemDoubleClicked(ui->conditionVariableList->item(duplicates));
    }
    ui->conditionVariableList->addItem(networks[index].getSelectedNodeName()+" = "+act->text());
    ui->conditionLabel->setVisible(true);
    ui->conditionVariableList->setVisible(true);
    QColor queryColor;
    queryColor.setHsv(20,30,250);
    networks[index].colorNode(networks[index].getSelectedNodeID(),queryColor);
    networks[index].removeHighlighting();
}

void MainWindow::context_Menu_InterventionValue_Selected(QAction* act)
{
    int index = ui->tabWidget->currentIndex();
    ui->interventionLabel->setText("setting");
    int duplicates = removeDuplicates(ui->interventionVariableList, networks[index].getSelectedNodeName(), 0);
    if (duplicates != -1){
        on_interventionVariableList_itemDoubleClicked(ui->interventionVariableList->item(duplicates));
    }
    duplicates = removeDuplicates(ui->queryVariableList,networks[index].getSelectedNodeName(),0);
    if (duplicates != -1){
        on_queryVariableList_itemDoubleClicked(ui->queryVariableList->item(duplicates));
    }
    ui->interventionVariableList->addItem(networks[index].getSelectedNodeName()+" = "+act->text());
    ui->interventionLabel->setVisible(true);
    ui->interventionVariableList->setVisible(true);
    networks[index].doIntervention();
    networks[index].removeHighlighting();
}

void MainWindow::context_Menu_AddEdge_selected(){
    int index = ui->tabWidget->currentIndex();
    ui->edgeAdReLabel->setText("with respect to");
    networks[index].initialiseEdgeAddition();
    networks[index].NodeForEdgeAdditionSelected(networks[index].getSelectedNodeID());
    networks[index].removeHighlighting();
}

void MainWindow::context_EdgeRemove_Selected(){
    int index = ui->tabWidget->currentIndex();
    networks[index].RemoveSelectedEdge();
    ui->edgeAdReLabel->setText("with respect to");
    ui->edgeAdReLabel->setVisible(true);
    ui->edgeAdditionsRemovalList->setVisible(true);
    ui->edgeAdditionsRemovalList->addItem("- "+networks[index].removedEdgeSourceName()+" "+networks[index].removedEdgeTargetName());
    networks[index].removeHighlighting();
}

void MainWindow::context_Menu_ShowMatrix_selected(){
    int index = ui->tabWidget->currentIndex();

    matrixPopup* mp = new matrixPopup();
    mp->setWindowTitle("CPT of "+networks[index].getSelectedNodeName());

    unsigned int rowCount = networks[index].getRowCountOfCurrentProbMatrix();
    unsigned int colCount = networks[index].getColCountOfCurrentProbMatrix();
    unsigned int parentCount = networks[index].getNumberOfParentsOfSelectedNode();
    mp->initaliseTable(rowCount+1, colCount+parentCount);

    mp->setRowNames(networks[index].getRowNamesOfProbabilityMatrix());
    mp->setColNames(networks[index].getColNamesOfProbabilityMatrix(),parentCount);
    mp->setParentNames(networks[index].getParentsOfSelectedNode());

    for (unsigned int row = 0; row < rowCount; row++){
        for (unsigned int col = 0; col < colCount; col++){
            mp->addData(row+1,col+parentCount,networks[index].getProbabilityAt(row,col));
        }
    }
    mp->adjust();
    mp->show();
    networks[index].removeHighlighting();
}

void MainWindow::resizeEvent(QResizeEvent *){
   if (not networks.empty()){
    networks[0].setWidth(MainWindow::width()-ui->dockWidget_5->width());
    networks[0].resizeNV(networks[0].getNVSizeHint());
   }
}

void MainWindow::on_Input_textChanged(const QString &arg1){
    if (not networks.empty()){
        if (arg1==""){
            on_deleteQueryButton_clicked();
            clearLabelsAndValueList();
        }
    }
}

QString MainWindow::buildQuery(){
    QString query = "?";
    int index = ui->tabWidget->currentIndex();
    if (networks[index].isArgMax()){
        query = query + " argmax (";
        for (int i = 0; i< ui->queryVariableList->count(); i++){
            query=query+" "+ui->queryVariableList->item(i)->text().split(" ")[1];
        }
        query = query + " )";
    }
    else{
        for (int i = 0; i< ui->queryVariableList->count(); i++){
            query=query+" "+ui->queryVariableList->item(i)->text();
        }
    }
    for (int j = 0; j< ui->conditionVariableList->count(); j++){
        query=query+" | "+ui->conditionVariableList->item(j)->text();
    }

    for (int h = 0; h< ui->interventionVariableList->count(); h++){
        query=query+" ! do "+ui->interventionVariableList->item(h)->text();
    }

    for (int k = 0; k< ui->edgeAdditionsRemovalList->count(); k++){
        query=query+" ! "+ui->edgeAdditionsRemovalList->item(k)->text();
    }
    return query;
}

void MainWindow::on_queryVariableList_itemDoubleClicked(QListWidgetItem* item)
{
    int index = ui->tabWidget->currentIndex();
    QColor queryColor;
    queryColor.setHsv(100,30,250);
    QStringList list = item->text().split(" ");
    if (list[0] == "argmax"){
      networks[index].removeNodeColor(list[1],queryColor);
    }
    else{
     networks[index].removeNodeColor(list[0],queryColor);
    }
    delete item;
    checkEmptyList(ui->queryVariableList,ui->queryLabel);
	checkAllEmpty();
}

void MainWindow::on_conditionVariableList_itemDoubleClicked(QListWidgetItem *item)
{
	int index = ui->tabWidget->currentIndex();
    QColor queryColor;
    queryColor.setHsv(20,30,250);
    networks[index].removeNodeColor(item->text().split(" ")[0],queryColor);
    delete item;
    checkEmptyList(ui->conditionVariableList,ui->conditionLabel);
	checkAllEmpty();
}

void MainWindow::on_interventionVariableList_itemDoubleClicked(QListWidgetItem *item)
{
    int index = ui->tabWidget->currentIndex();
    networks[index].removeDoIntervention(item->text().split(" ")[0]);
    delete item;
    checkEmptyList(ui->interventionVariableList,ui->interventionLabel);
	checkAllEmpty();
}

void MainWindow::on_edgeAdditionsRemovalList_itemDoubleClicked(QListWidgetItem *item)
{
    int index = ui->tabWidget->currentIndex();
    QStringList splitList = item->text().split(" ");
    if (splitList[0] == "-"){
        networks[index].reverseEdgeRemoval(splitList[1],splitList[2]);
    }
    else{
        networks[index].reverseEdgeAddition(splitList[1],splitList[2]);
    }
    delete item;
    checkEmptyList(ui->edgeAdditionsRemovalList,ui->edgeAdReLabel);
	checkAllEmpty();
}


void MainWindow::on_actionSaveSession_triggered()
{
    dataStorage dataStore;
    QFileDialog dialog;
    dialog.setDefaultSuffix("cts");
    dialog.exec();
    QString filename = dialog.selectedFiles()[0];
    dataStore.saveSession(networks,filename);
    ui->Output->addItem("Session saved");
    ui->Output->scrollToBottom();
}



void MainWindow::on_actionLoad_Session_triggered()
{
    dataStorage dataStore;
    QString filename = QFileDialog::getOpenFileName(this,tr("Load session file"),path,tr("*cts"));
    int index = 0;
    if (filename != ""){
        dataStore.loadSession(filename);
        for (unsigned int i = 0; i< dataStore.getNumberOfLoadedNetworks(); i++){
            index = generateNetworkInstance();
            loadNAorTGF(dataStore.getNAorTGf(i),index);
            if (boost::filesystem::extension(dataStore.getNAorTGf(i).toStdString())==".na"){
                loadSif(dataStore.getSif(i),index);
            }
            visualise(index);
			networks[index].setDeselectedSamples(dataStore.getDeSelectedData(index));
            loadSamples(dataStore.getData(i),dataStore.getDiscretiseControl(i),index);
            networks[index].setQMA(dataStore.getQma(i));
        }
        ui->Output->addItem("Session loaded");
        ui->Output->scrollToBottom();
        if (networks.back().getNumberOfQueries() > 0){
            ui->actionCreate_Batchfile->setEnabled(true);
            ui->actionCreate_Batchfile_2->setEnabled(true);
            ui->loadPreviousQueryButton->setEnabled(true);
            ui->loadPreviousQueryButton_2->setEnabled(true);
            loadQueriesToHistoryWindow(index);
        }
    }
}

void MainWindow::loadQueriesToHistoryWindow(int index){
    ui->queryHistory->clear();
    for (unsigned int queryNumber = 0; queryNumber < networks[index].getNumberOfQueries(); queryNumber++){
        ui->queryHistory->addItem(networks[index].getQuery(queryNumber));
    }
}


void MainWindow::on_actionCreate_Batchfile_triggered()
{
    dataStorage dataStore;
    QFileDialog dialog;
    dialog.setDefaultSuffix("txt");
    dialog.exec();
    QString filename = dialog.selectedFiles()[0];
    int index = ui->tabWidget->currentIndex();
    dataStore.createQueryBatchFile(networks[index],filename);
    ui->Output->addItem("Batch file created");
    ui->Output->scrollToBottom();
}

void MainWindow::on_actionExecute_Batchfile_triggered()
{
    std::ifstream input;
    QString filename = QFileDialog::getOpenFileName(this,tr("Load batch file"),path,tr("*txt"));
    if (filename != ""){
        input.open(filename.toStdString());
        std::string line;
        int index = ui->tabWidget->currentIndex();
        std::pair<float,std::vector<std::string>> result;
        try{
            ui->Output->addItem("Evaluating queries in: "+filename);
            while(! input.eof()){
                std::getline(input,line);
                if (line != ""){
                    result = networks[index].calculate(QString::fromStdString(line));
                    ui->Output->addItem(QString::fromStdString(line)+": "+QString::number(result.first));
                    QString temp="";
                    for (auto& value :result.second){
                        temp+=QString::fromStdString(value)+" ";
                        }
                    if (temp != ""){
                        ui->Output->addItem(temp);
                     }
                }
            }
        }
        catch(std::exception& e){
            ui->Output->addItem(e.what());
        }
    }
    else{
        ui->Output->addItem("No file specified");
    }
    ui->Output->scrollToBottom();

}


void MainWindow::on_queryHistory_doubleClicked(const QModelIndex &index)
{
    ui->Input->clear();
    int networkIndex = ui->tabWidget->currentIndex();
    networks[networkIndex].removeHighlighting();
    networks[networkIndex].restoreOriginalNetworkRepresentation();
    ui->Input->setText(networks[networkIndex].getQuery(index.row()));
    writeListWidget(ui->queryVariableList,ui->queryLabel,networks[networkIndex].getQueryItems(index.row()));
    writeListWidget(ui->conditionVariableList, ui->conditionLabel, networks[networkIndex].getConditionItems(index.row()));
    writeListWidget(ui->interventionVariableList, ui->interventionLabel, networks[networkIndex].getInterventionItems(index.row()));
    writeListWidget(ui->edgeAdditionsRemovalList, ui->edgeAdReLabel, networks[networkIndex].getEdgeAddRemItems(index.row()));
    ui->Input->setFocus();
}
