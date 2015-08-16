#include "QueryView.h"
#include "NetworkInstance.h"
#include "NodeGui.h"

#include "../core/Parser.h"

#include "ui_QueryView.h"

#include <QtCore/QDebug>
#include <QtCore/QMessageLogger>

QueryView::QueryView(QWidget* parent)
	: QWidget(parent),
	  ui(new Ui::QueryView),
	  net_(nullptr)
{
	ui->setupUi(this);

	clear();
}

QueryView::~QueryView() { delete ui; }

void QueryView::clear()
{
	clearResults_();

	ui->conditionLabel->hide();
	ui->conditionVariableList->hide();
	ui->conditionVariableList->clear();

	ui->edgeAdReLabel->hide();
	ui->edgeAdditionsRemovalList->hide();
	ui->edgeAdditionsRemovalList->clear();

	ui->interventionLabel->hide();
	ui->interventionVariableList->hide();
	ui->interventionVariableList->clear();

	ui->queryLabel->setText(tr("Right click on a Node to build a query"));
	ui->queryVariableList->hide();
	ui->queryVariableList->clear();

	ui->Input->clear();
	ui->loadNextQuery->setEnabled(false);
	ui->loadPreviousQuery->setEnabled(false);
	ui->deleteQuery->setEnabled(false);
}

void QueryView::newQuery()
{
	clear();

	if(net_) {
		net_->removeHighlighting();
		net_->restoreOriginalNetworkRepresentation();
		net_->setArgMax(false);
	}

	checkQueriesLeft();
}

unsigned int QueryView::numQueryVariables() const
{
	return ui->queryVariableList->count();
}

void QueryView::clearResults_()
{
	clearResult_();
	clearValue_();
}

void QueryView::updateQuery(unsigned int index)
{
	net_->removeHighlighting();
	net_->restoreOriginalNetworkRepresentation();

	displayQuery_(index);

	ui->queryLabel->setText(tr("Calculating probability of"));

	checkQueriesLeft();
}

void QueryView::updateQuery_()
{
	if(net_->getQMA().getNumberOfQueries() > 0) {
		updateQuery(queryIndex());
	}
}

void QueryView::setNetworkInstance(NetworkInstance* net)
{
	if(net == net_) {
		return;
	}

	if(net_ != nullptr) {
		disconnect(net_);
	}

	net_ = net;

	if(net_ != nullptr) {
		connect(net_, SIGNAL(edgeAdded(int, int)), this,
		        SLOT(edgeAdded(int, int)));
		connect(net_, SIGNAL(edgeRemoved(int, int)), this,
		        SLOT(edgeRemoved(int, int)));

		updateQuery_();
	} else {
		clear();
	}
}

void QueryView::conditionOn(unsigned int nodeId, const QString& value)
{
	ui->conditionLabel->setText(tr("given that"));
	int duplicates = removeDuplicates(ui->conditionVariableList,
	                                  net_->getSelectedNodeName(), 0);
	if(duplicates != -1) {
		on_conditionVariableList_itemDoubleClicked(
		    ui->conditionVariableList->item(duplicates));
	}
	ui->conditionVariableList->addItem(net_->getSelectedNodeName() + " = " +
	                                   value);
	ui->conditionLabel->show();
	ui->conditionVariableList->show();
	net_->colorNode(nodeId, QColor::fromHsv(20, 30, 250));
	net_->removeHighlighting();

	updateQueryText_();
}

void QueryView::interventionOn(unsigned int nodeId, const QString& value)
{
	ui->interventionLabel->setText(tr("setting"));

	const QString& nodeName = net_->getSelectedNodeName();

	int duplicates =
	    removeDuplicates(ui->interventionVariableList, nodeName, 0);
	if(duplicates != -1) {
		on_interventionVariableList_itemDoubleClicked(
		    ui->interventionVariableList->item(duplicates));
	}
	duplicates = removeDuplicates(ui->queryVariableList, nodeName, 0);
	if(duplicates != -1) {
		on_queryVariableList_itemDoubleClicked(
		    ui->queryVariableList->item(duplicates));
	}
	ui->interventionVariableList->addItem(nodeName + " = " + value);
	ui->interventionLabel->show();
	ui->interventionVariableList->show();
	net_->doIntervention();
	net_->removeHighlighting();

	updateQueryText_();
}

void QueryView::edgeRemoved(int source, int target)
{
	addRemovalAddition_("-", source, target);
}

void QueryView::edgeAdded(int source, int target)
{
	addRemovalAddition_("+", source, target);

	if(!net_->edgeAddition()) {
		ui->Input->setFocus();
	}
}

void QueryView::addRemovalAddition_(const char* prefix, unsigned int source,
                                    unsigned int target)
{
	ui->edgeAdReLabel->setText("with respect to");
	ui->edgeAdReLabel->show();
	ui->edgeAdditionsRemovalList->show();
	ui->edgeAdditionsRemovalList->addItem(QString("%1 %2 %3").arg(
	    prefix,
	    QString::fromStdString(
	        net_->getController().getNetwork().getNode(source).getName()),
	    QString::fromStdString(
	        net_->getController().getNetwork().getNode(target).getName())));
	net_->removeHighlighting();
	updateQueryText_();
}

void QueryView::computeArgMax(unsigned int nodeId)
{
	const QString& nodeName = net_->getSelectedNodeName();

	net_->setArgMax(true);
	ui->queryLabel->setText(
	    tr("Calculating the most probable value assignment for"));
	int duplicates = removeDuplicates(ui->queryVariableList, nodeName, 1);
	if(duplicates != -1) {
		on_queryVariableList_itemDoubleClicked(
		    ui->queryVariableList->item(duplicates));
	}
	duplicates = removeDuplicates(ui->interventionVariableList, nodeName, 0);
	if(duplicates != -1) {
		on_interventionVariableList_itemDoubleClicked(
		    ui->interventionVariableList->item(duplicates));
	}
	duplicates = removeDuplicates(ui->conditionVariableList, nodeName, 0);
	if(duplicates != -1) {
		on_conditionVariableList_itemDoubleClicked(
		    ui->conditionVariableList->item(duplicates));
	}
	ui->queryVariableList->addItem("argmax " + nodeName);
	ui->queryVariableList->setVisible(true);
	net_->colorNode(nodeId, QColor::fromHsv(100, 30, 250));
	net_->removeHighlighting();

	updateQueryText_();
}

void QueryView::computeProbability(unsigned int nodeId, const QString& value)
{
	QString nodeName = net_->getSelectedNodeName();
	ui->queryLabel->setText(tr("Calculating probability of"));
	int duplicates =
	    removeDuplicates(ui->interventionVariableList, nodeName, 0);
	if(duplicates != -1) {
		on_interventionVariableList_itemDoubleClicked(
		    ui->interventionVariableList->item(duplicates));
	}
	duplicates = removeDuplicates(ui->queryVariableList, nodeName, 0);
	if(duplicates != -1) {
		on_queryVariableList_itemDoubleClicked(
		    ui->queryVariableList->item(duplicates));
	}
	duplicates = removeDuplicates(ui->queryVariableList, nodeName, 1);
	if(duplicates != -1) {
		on_queryVariableList_itemDoubleClicked(
		    ui->queryVariableList->item(duplicates));
	}
	ui->queryVariableList->addItem(nodeName + " = " + value);
	ui->queryVariableList->setVisible(true);
	net_->colorNode(nodeId, QColor::fromHsv(100, 30, 250));
	net_->removeHighlighting();
	updateQueryText_();
	
}

QString QueryView::buildQuery() const
{
	QString query = "?";
	if(net_->isArgMax()) {
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

void QueryView::on_deleteQuery_clicked() { emit queryDeleted(queryIndex()); }

void QueryView::on_loadNextQuery_clicked()
{
	clear();

	net_->getQMA().getSubsequentQuery();
	updateQuery_();

	emit queryChanged(queryIndex());
}

void QueryView::on_loadPreviousQuery_clicked()
{
	clear();

	net_->getQMA().getPreviousQuery();
	updateQuery_();

	emit queryChanged(queryIndex());
}

void QueryView::on_newQuery_clicked() { newQuery(); }

void QueryView::on_executeQuery_clicked()
{
	QString query = buildQuery();

	if(query != ui->Input->text()) {
		query = ui->Input->text();
	}

	std::pair<float, std::vector<std::string>> result;
	try {
		result = net_->calculate(ui->Input->text().toStdString());
	} catch(std::invalid_argument& e) {
		emit newLogMessage(QString::fromStdString(e.what()));
		return;
	}

	emit newLogMessage(ui->Input->text() + ": " +
	                   QString::number(result.first));
	showResult_(QString::number(result.first));

	QString temp = "";
	for(unsigned int i = 0; i < result.second.size(); i++) {
		auto nodeName = ui->queryVariableList->item(i)->text().split(" ")[1];
		temp += nodeName + "=" + QString::fromStdString(result.second[i]) + " ";
	}

	if(temp != "") {
		emit newLogMessage(temp);
		showValue_(temp);
	}

	net_->getQMA().storeQuery(ui->Input->text(),
	                          getVector(ui->queryVariableList),
	                          getVector(ui->conditionVariableList),
	                          getVector(ui->interventionVariableList),
	                          getVector(ui->edgeAdditionsRemovalList));

	checkQueriesLeft();
	net_->removeHighlighting();

	emit queryExecuted(queryIndex());
}

void QueryView::on_queryVariableList_itemDoubleClicked(QListWidgetItem* item)
{
	QStringList list = item->text().split(" ");

	int index = (list[0] == "argmax") ? 1 : 0;
	net_->removeNodeColor(list[index], QColor::fromHsv(100, 30, 250));

	delete item;
	checkEmptyList(ui->queryVariableList, ui->queryLabel);
	checkAllEmpty();

	updateQueryText_();
}

void
QueryView::on_conditionVariableList_itemDoubleClicked(QListWidgetItem* item)
{
	net_->removeNodeColor(item->text().split(" ")[0],
	                      QColor::fromHsv(20, 30, 250));
	delete item;
	checkEmptyList(ui->conditionVariableList, ui->conditionLabel);
	checkAllEmpty();

	updateQueryText_();
}

void
QueryView::on_interventionVariableList_itemDoubleClicked(QListWidgetItem* item)
{
	net_->removeDoIntervention(item->text().split(" ")[0]);
	delete item;
	checkEmptyList(ui->interventionVariableList, ui->interventionLabel);
	checkAllEmpty();

	updateQueryText_();
}

void
QueryView::on_edgeAdditionsRemovalList_itemDoubleClicked(QListWidgetItem* item)
{
	QStringList splitList = item->text().split(" ");
	if(splitList[0] == "-") {
		net_->reverseEdgeRemoval(splitList[1], splitList[2]);
	} else {
		net_->reverseEdgeAddition(splitList[1], splitList[2]);
	}
	delete item;
	checkEmptyList(ui->edgeAdditionsRemovalList, ui->edgeAdReLabel);
	checkAllEmpty();

	updateQueryText_();
}

void QueryView::on_Input_textChanged(const QString& arg1)
{
	if(!net_ || arg1 == "") {
		clear();
		ui->Input->setStyleSheet("");
		return;
	}
	Parser parser(arg1.toStdString(), net_->getController());

	try {
		auto qe = parser.parseQuery();

		if(qe.getArgMaxIds().empty()) {
			writeListWidget(ui->queryVariableList, ui->queryLabel,
			                qe.getNonInterventionIds(),
			                qe.getNonInterventionValues());
			net_->setArgMax(false);
		} else {
			ui->queryVariableList->clear();
			ui->queryVariableList->show();
			ui->queryLabel->show();
			for(const auto& id : qe.getArgMaxIds()) {
				ui->queryVariableList->addItem(
				    QStringLiteral("argmax ") +
				    QString::fromStdString(net_->getNodeName(id)));
			}
			net_->setArgMax(true);
		}

		writeListWidget(ui->interventionVariableList, ui->interventionLabel,
		                qe.getInterventionIds(), qe.getInterventionValues());
		writeListWidget(ui->conditionVariableList, ui->conditionLabel,
		                qe.getConditionIds(), qe.getConditionValues());

		for(auto&& addition : qe.getEdgeAdditionIds()) {
			addRemovalAddition_("+", addition.first, addition.second);
		}

		for(auto&& addition : qe.getEdgeRemovalIds()) {
			addRemovalAddition_("-", addition.first, addition.second);
		}

		ui->Input->setStyleSheet("QLineEdit{background:#33aa33;}");
	} catch(const std::invalid_argument&) {
		ui->Input->setStyleSheet("QLineEdit{background:#aa3333;}");
	}
}

void QueryView::checkAllEmpty()
{
	if(ui->queryVariableList->count() == 0 &&
	   ui->conditionVariableList->count() == 0 &&
	   ui->interventionVariableList->count() == 0 &&
	   ui->edgeAdditionsRemovalList->count() == 0) {
		newQuery();
	}

	ui->Input->setFocus();
}

unsigned int QueryView::queryIndex() const
{
	if(!net_) {
		qCritical("Trying to obtain current query from uninitialized view.");
	}

	return net_->getQMA().getCurrentQuery();
}

void QueryView::checkQueriesLeft()
{
	if(net_) {
		ui->loadPreviousQuery->setEnabled(!(net_->getQMA().isBeginning()));
		ui->loadNextQuery->setEnabled(!(net_->getQMA().isFinal()));
	}
}

void QueryView::checkEmptyList(QListWidget* widget, QLabel* label) const
{
	if(widget->count() == 0) {
		widget->hide();
		label->hide();
	}
}

std::vector<QString> QueryView::getVector(QListWidget* widget) const
{
	std::vector<QString> temp;
	temp.reserve(widget->count());

	for(int i = 0; i < widget->count(); i++) {
		temp.push_back(widget->item(i)->text());
	}

	return temp;
}

void QueryView::writeListWidget(QListWidget* widget, QLabel* label,
                                const std::vector<QString>& vec) const
{
	widget->clear();

	widget->setVisible(!vec.empty());
	label->setVisible(!vec.empty());

	for(auto&& item : vec) {
		widget->addItem(item);
	}
}

void QueryView::writeListWidget(QListWidget* widget, QLabel* label,
                                const std::vector<unsigned int>& ids,
                                const std::vector<int>& values) const
{
	widget->clear();

	widget->setVisible(!ids.empty());
	label->setVisible(!ids.empty());

	for(auto nodeId : ids) {
		const auto& node = net_->getController().getNetwork().getNode(nodeId);
		if(values[nodeId] != -1 &&
		   static_cast<size_t>(values[nodeId]) < node.getValueNames().size()) {
			widget->addItem(
			    QString::fromStdString(node.getName()) + " = " +
			    QString::fromStdString(node.getValueNamesProb()[values[nodeId]]));
		} else {
			qCritical() << "Invalid value" << values[nodeId]
			            << "supplied for node" << node.getName().c_str();
		}
	}
}

int QueryView::removeDuplicates(QListWidget* widget, const QString& nodeName,
                                unsigned int index) const
{
	for(int i = 0; i < widget->count(); i++) {
		if(nodeName == widget->item(i)->text().split(" ")[index]) {
			return i;
		}
	}

	return -1;
}

void QueryView::updateQueryText_() { ui->Input->setText(buildQuery()); }

void QueryView::showResult_(const QString& text)
{
	ui->probResultLabel->setText(text);
	ui->probResultLabel->show();
	ui->probLabel->show();
}

void QueryView::clearResult_()
{
	ui->probLabel->hide();
	ui->probResultLabel->hide();
	ui->probResultLabel->clear();
}

void QueryView::showValue_(const QString& text)
{
	ui->valueMaxLabel->setText(text);
	ui->valueMaxLabel->show();
	ui->valueLabel->show();
}

void QueryView::clearValue_()
{
	ui->valueLabel->hide();
	ui->valueMaxLabel->hide();
	ui->valueMaxLabel->clear();
}

void QueryView::displayQuery_(unsigned int index)
{
	const auto& queryManager = net_->getQMA();

	if(index >= queryManager.getNumberOfQueries()) {
		qCritical("Trying to display invalid query with index %d", index);
		return;
	}

	ui->Input->setText(queryManager.getQuery(index));
	writeListWidget(ui->queryVariableList, ui->queryLabel,
	                queryManager.getQueryItems(index));
	writeListWidget(ui->conditionVariableList, ui->conditionLabel,
	                queryManager.getConditionItems(index));
	writeListWidget(ui->interventionVariableList, ui->interventionLabel,
	                queryManager.getInterventionItems(index));
	writeListWidget(ui->edgeAdditionsRemovalList, ui->edgeAdReLabel,
	                queryManager.getEdgeAddRemItems(index));
	ui->Input->setFocus();
}
