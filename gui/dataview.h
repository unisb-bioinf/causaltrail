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
    explicit dataview(QWidget *parent = 0, QString samples="");
    ~dataview();
    void adjust();

private slots:
    void on_rejectButton_clicked();
    void on_submitButton_clicked();

    void toogle_all_clicked(int state);
signals:
    void dataSubmitted(QString, std::vector<unsigned int>);
    void dataRejected();

private:
    QString file_;
    Ui::dataview *ui;
    std::vector<QCheckBox*> boxes_;
    void loadRowNames(Matrix<std::string> &mat);
    void loadData(Matrix<std::string> &mat);
    void adjustRowColCount(Matrix<std::string> &mat);
    void addCheckBoxes(Matrix<std::string> &mat);
    std::vector<unsigned int> getDeselectedSamples();
};

#endif // DATAVIEW_H
