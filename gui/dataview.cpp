#include "dataview.h"
#include "ui_dataview.h"


dataview::dataview(QWidget *parent, QString samples) :
    QWidget(parent),file_(samples),
    ui(new Ui::dataview)
{
    ui->setupUi(this);
    ui->matrixVis->showGrid();
    Matrix<std::string> data (samples.toStdString(),false,true);
    adjustRowColCount(data);
    loadRowNames(data);
    loadData(data);
    addCheckBoxes(data);
}

void dataview::adjustRowColCount(const Matrix<std::string>& mat){
    ui->matrixVis->setRowCount(mat.getRowCount()+1);
    ui->matrixVis->setColumnCount(mat.getColCount()+1);
}

void dataview::loadRowNames(const Matrix<std::string>& mat){
    QFont fnt;
    fnt.setBold(true);
    unsigned int row=1;
    for (const std::string& name : mat.getRowNames()){
        QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(name));
        item->setFont(fnt);
        ui->matrixVis->setItem(row++,0,item);
    }
}

void dataview::loadData(const Matrix<std::string>& mat){
    for (unsigned int row = 0; row < mat.getRowCount(); row++){
        for (unsigned int col = 0; col < mat.getColCount(); col++){
            QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(mat(col,row)));
            ui->matrixVis->setItem(row+1, col+1, item);
        }
    }
}

void dataview::addCheckBoxes(const Matrix<std::string>& mat){
    QCheckBox* item = new QCheckBox(this);
    connect(item,SIGNAL(stateChanged(int)),this,SLOT(toogle_all_clicked(int)));
    QVBoxLayout* layout = new QVBoxLayout;
    QWidget* w = new QWidget;
    layout->addWidget(item);
    layout->setContentsMargins(5,0,0,0);
    w->setLayout(layout);
    item->setText("Toogle all");
    item->setChecked(true);
    boxes_.push_back(item);
    ui->matrixVis->setCellWidget(0,0,w);
    for (unsigned int col = 1; col <= mat.getColCount(); col++){
        QCheckBox* item = new QCheckBox(this);
        boxes_.push_back(item);
        QVBoxLayout* layout = new QVBoxLayout;
        QWidget* w = new QWidget;
        layout->addWidget(item);
        layout->setContentsMargins(5,0,0,0);
        w->setLayout(layout);
        item->setText("Sample "+QString::number(col));
        item->setChecked(true);
        ui->matrixVis->setCellWidget(0,col,w);
    }
}

void dataview::adjust(){
    ui->matrixVis->resizeColumnsToContents();
    ui->matrixVis->adjustSize();
    adjustSize();
}

void dataview::toogle_all_clicked(int state){
    for (unsigned int i = 0; i< boxes_.size(); i++){
        boxes_[i]->setChecked(state);
    }
}

const std::vector<unsigned int> dataview::getDeselectedSamples(){
    std::vector<unsigned int> temp;
    for (unsigned int i = 1; i< boxes_.size(); i++){
        if (not boxes_[i]->isChecked()){
            temp.push_back(i);
        }
    }
    return temp;
}


dataview::~dataview()
{
    delete ui;
}


void dataview::on_rejectButton_clicked()
{
    emit dataRejected();
    close();
}

void dataview::on_submitButton_clicked()
{
	std::vector<unsigned int> selected = getDeselectedSamples();
	if (selected.size() == boxes_.size()){
		throw std::invalid_argument("No samples selected");
	}
	else{
	    emit dataSubmitted(file_,selected);
	}
    close();
}
