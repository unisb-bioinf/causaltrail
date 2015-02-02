#include "matrixpopup.h"
#include "ui_matrixpopup.h"

matrixPopup::matrixPopup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::matrixPopup)
{
    ui->setupUi(this);
    ui->matrixVis->showGrid();
}

void matrixPopup::initaliseTable(unsigned int rowCount, unsigned int colCount)
{
    ui->matrixVis->setRowCount(rowCount);
    ui->matrixVis->setColumnCount(colCount);
}

void matrixPopup::setRowNames(const std::vector<std::string> &rowNames)
{
    int row = 1;
    int col;
    QFont fnt;
    fnt.setPointSize(10);
    for (const std::string& name : rowNames){
        col=0;
        QString qname = QString::fromStdString(name);
        for (auto sname : qname.split(",")){
            QTableWidgetItem* item = new QTableWidgetItem(sname);
            item->setFont(fnt);
            ui->matrixVis->setItem(row,col++,item);
        }
        row++;
    }
}

void matrixPopup::setColNames(const std::vector<std::string> &colNames, unsigned int shift)
{
    int col = shift;
    QFont fnt;
    fnt.setBold(true);
    fnt.setPointSize(11);
    for (const std::string& name : colNames){
        QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(name));
        item->setFont(fnt);
        ui->matrixVis->setItem(0,col++,item);
    }
}

void matrixPopup::setParentNames(const std::vector<std::string>& names){
    QFont fnt;
    fnt.setPointSize(10);
    fnt.setItalic(true);
    int col = 0;
    for (const auto& name : names){
        QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(name));
        item->setFont(fnt);
        ui->matrixVis->setItem(0,col++,item);
    }
}

void matrixPopup::addData(unsigned int row, unsigned int col, float data)
{
    QFont fnt;
    fnt.setPointSize(10);
    QTableWidgetItem* item = new QTableWidgetItem(QString::number(data));
    item->setFont(fnt);
    ui->matrixVis->setItem(row,col,item);
}

void matrixPopup::adjust(){
    ui->matrixVis->resizeColumnsToContents();
    ui->matrixVis->adjustSize();
    adjustSize();
}

matrixPopup::~matrixPopup()
{
    delete ui;
}
