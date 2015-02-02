#ifndef MATRIXPOPUP_H
#define MATRIXPOPUP_H

#include <QWidget>

namespace Ui {
class matrixPopup;
}

class matrixPopup : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief matrixPopup
     * Constructor of the MatrixPopup class
     * @param parent The parent widget
     */
    explicit matrixPopup(QWidget *parent = 0);

    /**
     * @brief initaliseTable
     * initialises the matrix view to be fit to the size of the probability matrix of the node of interest
     * @param rowCount Number of rows
     * @param colCount Number of columns
     */
    void initaliseTable(unsigned int rowCount, unsigned int colCount);

    /**
     * @brief setRowNames
     * Sets the rowNames for the matrix
     * @param rowNames Vector containing the rowNames
     */
    void setRowNames(const std::vector<std::string>& rowNames);

    /**
     * @brief setColNames
     * Sets the colNames for the matrix
     * @param colNames Vector containing the colNames
     * @param shift Number of parents of this node.
     */
    void setColNames(const std::vector<std::string>& colNames, unsigned int shift);

    /**
     * @brief addData
     * Writes the provided data to the matrix
     * @param row Index of the row
     * @param col Index of the column
     * @param data Value to write
     */
    void addData(unsigned int row, unsigned int col, float data);

    /**
     * @brief adjust
     * Reimplemenation of the adjust() method of QWidget
     */
    void adjust();

    /**
     * @brief setParentNames
     * Sets the name(s) of the parental node(s) of the current node
     * @param names Vector containing the name(s) of the parental node(s)
     */
    void setParentNames(const std::vector<std::string>& names);

    /**
     * @brief ~matrixPopup
     * Destructor of this class
     */
    ~matrixPopup();

private:
    /**
     * @brief ui
     * Graphical User Interface
     */
    Ui::matrixPopup *ui;
};

#endif // MATRIXPOPUP_H
