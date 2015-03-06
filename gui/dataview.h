#ifndef DATAVIEW_H
#define DATAVIEW_H

#include "../core/Matrix.h"
#include <QWidget>
#include <QCheckBox>


namespace Ui {
class dataview;
}

class dataview : public QWidget
{
    Q_OBJECT

public:
	
	/**
	 * @brief dataview
	 * Constructs a dataview object
	 * @param parent, the parent widget
	 * @param samples, name of the file containing samples
	 * @return a dataview object
	 */
    explicit dataview(QWidget *parent = 0, QString samples="");
	
	/**
	 * @brief ~dataview
	 * Destructor of the dataview object
	 */
    ~dataview();
	
	/**
	 * @brief adjust
	 * Reimplementation of the QWidget::adjust() method
	 */
    void adjust();

private slots:

	/**
	 * @brief on_rejectButton_clicked
	 *
	 */
    void on_rejectButton_clicked();

	/**
	 * @brief on_submitButton_clicked
	 *
	 */
    void on_submitButton_clicked();

	/**
	 * @brief toogle_all_clicked
	 *
	 * @param state, 1 if a box is checked, 0 if a box is not checked
	 */
    void toogle_all_clicked(int state);

signals:

	/**
	 * @brief dataSubmitted
	 * This signal is emitted if the button "Load data" is clicked
	 * @param QString name of the file containing samples
	 * @param std::vector<unsigned int> vector containing deselected samples
	 */
    void dataSubmitted(const QString, const std::vector<unsigned int>);

	/**
	 * @brief dataRejected
	 * This signal is emitted if the button "Do not load data" is clicked
	 */
    void dataRejected();

private:
	//Name of the file containing samples
    QString file_;
	//UserInterface
    Ui::dataview *ui;
	//Vector containing pointers to the CheckBoxes
    std::vector<QCheckBox*> boxes_;

	/**
	 * @brief loadRowNames
	 * Extracts the row names of the observations matrix and adds them
	 * to the visualisation
	 * @param mat, Reference to the matrix containing the observations
	 */
    void loadRowNames(const Matrix<std::string> &mat);

	/**
	 * @brief loadData
	 * Extracts the data of the observations matrix and adds it to
	 * the visualisation
	 * @param mat, Reference to the matrix containing the observations
	 */
    void loadData(const Matrix<std::string> &mat);

	/**
	 * @brief adjustRowColCount
	 * Resizes the visualisation according to the size of the observations matrix
	 * @param mat, Reference to the matrix containing the observations
	 */
    void adjustRowColCount(const Matrix<std::string> &mat);

	/**
	 * @brief addCheckBoxes
	 * Adds QCheckBox objects to the visualisation
	 * @param mat, Reference to the matrix containing the observations
	 */
    void addCheckBoxes(const Matrix<std::string> &mat);

	/**
	 * @brief getDeselectedSamples
	 * Creates a vector containing the deselected samples by iterating through the check boxes
	 * @param mat, Reference to the matrix containing the observations
	 * @return std::vector<unsigned int> vector containing the index of the deselected samples
	 */
    const std::vector<unsigned int> getDeselectedSamples();
};

#endif // DATAVIEW_H
