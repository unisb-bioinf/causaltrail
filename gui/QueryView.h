#ifndef CAUSALTRAIL_QUERY_VIEW_H
#define CAUSALTRAIL_QUERY_VIEW_H

#include <QtWidgets/QWidget>

namespace Ui {
	class QueryView;
}

class NetworkInstance;
class NodeGui;

class QLabel;
class QListWidget;
class QListWidgetItem;
class QModelIndex;

class QueryView : public QWidget
{
	Q_OBJECT
	public:
	QueryView(QWidget* parent = nullptr);
	virtual ~QueryView();

	QString buildQuery() const;
	void setNetworkInstance(NetworkInstance* net);
	void clear();

	unsigned int numQueryVariables() const;

	public slots:
	void newQuery();
	void updateQuery(unsigned int);
	void conditionOn(unsigned int nodeId, const QString& value);
	void interventionOn(unsigned int nodeId, const QString& value);
	void computeProbability(unsigned int nodeId, const QString& value);
	void computeArgMax(unsigned int nodeId);
	void edgeRemoved(int source, int target);
	void edgeAdded(int source, int target);

	protected slots:
	void on_newQuery_clicked();
	void on_deleteQuery_clicked();
	void on_loadPreviousQuery_clicked();
	void on_loadNextQuery_clicked();
	void on_executeQuery_clicked();

	void on_Input_textChanged(const QString& text);

	void on_queryVariableList_itemDoubleClicked(QListWidgetItem* item);
	void on_conditionVariableList_itemDoubleClicked(QListWidgetItem* item);
	void on_interventionVariableList_itemDoubleClicked(QListWidgetItem* item);
	void on_edgeAdditionsRemovalList_itemDoubleClicked(QListWidgetItem* item);

	signals:
	void queryDeleted(unsigned int);
	void queryExecuted(unsigned int);
	void queryModified(unsigned int);
	void queryChanged(unsigned int);
	void newLogMessage(const QString& msg);

	private:
	Ui::QueryView* ui;
	NetworkInstance* net_;

	void checkAllEmpty();
	unsigned int queryIndex() const;
	void checkQueriesLeft();

	// Auxillary functions
	void checkEmptyList(QListWidget* widget, QLabel* label) const;
	std::vector<QString> getVector(QListWidget* widget) const;

	/**
	 * @brief writeListWidget
	 * Adds the elements of vector vec to the given QListWidget and adapts the visiblity of both the QListWidget and the related QLabel
	 * @param widget Pointer to a QListWidget
	 * @param label Pointer to a QLabel
	 * @param vec Reference to a vector containing QStrings
	 */
	void writeListWidget(QListWidget* widget, QLabel* label, const std::vector<QString>& vec) const;

	/**
	 * @brief removeDuplicates
	 * Checks if the given QListWidget contains the given node
	 * @param widget Pointer to a QListWidget
	 * @param nodeName Name of the query node
	 * @param index Position of the line in the QListWidgets that should be compared to the query name
	 * @return -1 if there is no duplicate, otherwise the index of the duplicate
	 */
	int removeDuplicates(QListWidget* widget, const QString& nodeName, unsigned int index) const;
    void updateQuery_();
    void updateQueryText_();

	void clearResults_();
	void clearResult_();
	void clearValue_();
	void showResult_(const QString& text);
	void showValue_(const QString& text);

	void displayQuery_(unsigned int);
    void addRemovalAddition_(const char* prefix, unsigned int source, unsigned int target);
};

#endif // CAUSALTRAIL_QUERY_VIEW_H
