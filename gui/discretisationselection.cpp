#include "discretisationselection.h"
#include "ui_discretisationselection.h"
#include "QFileDialog"

discretisationSelection::discretisationSelection(QWidget* parent)
    : QDialog(parent), index_(-1), ui(new Ui::DiscretisationSelection)
{
    ui->setupUi(this);

}

void discretisationSelection::show(const QString& path, const QString& samples,
                              int index)
{
	path_ = path;
	samples_ = samples;
	index_ = index;

	adaptGUIToData();
	QDialog::show();
}

const QString& discretisationSelection::samples() const { return samples_; }

Discretiser::Discretisations discretisationSelection::control() const
{
	Discretiser::Discretisations disc;
	disc.reserve(ui->gridLayout->rowCount() - 1);

	for (int i = 1; i < ui->gridLayout->rowCount(); i++){
		disc.emplace_back(i - 1, boxes_[i - 1]->currentIndex(),
		                  optionalValues_[i - 1]->text().toFloat());
	}

	return disc;
}

int discretisationSelection::index() const { return index_; }

void discretisationSelection::adaptGUIToData(){
    QLineEdit* featureName;
    methodComboBox* methodSelection;
    QLineEdit* optionalValue;
    Matrix<std::string> originalData(samples_.toStdString(),false,true);
    for (unsigned int i = 1; i < originalData.getRowCount()+1; i++){
        featureName = newFeatureName(QString::fromStdString(originalData.getRowNames()[i-1]));
        methodSelection = newMethodSelection((i-1));
        optionalValue = newOptionalValue();
        ui->gridLayout->addWidget(featureName,i,0,0);
        ui->gridLayout->addWidget(methodSelection,i,1,0);
        boxes_.push_back(methodSelection);
        ui->gridLayout->addWidget(optionalValue,i,2,0);
        optionalValues_.push_back(optionalValue);
    }
    adjustSize();
    update();
}

discretisationSelection::~discretisationSelection()
{
    delete ui;
}

QLineEdit* discretisationSelection::newFeatureName(QString name)
{
    QLineEdit* featureName = new QLineEdit();
    featureName->setText(name);
    featureName->setReadOnly(true);
    return featureName;
}

methodComboBox* discretisationSelection::newMethodSelection(unsigned int i)
{
    methodComboBox* methodSelection = new methodComboBox(i);
    connect(methodSelection,SIGNAL(newMethodSelected(uint,int)),this,SLOT(box_Index_Changed(uint,int)));
    return methodSelection;
}

QLineEdit *discretisationSelection::newOptionalValue()
{
    QLineEdit* optionalValue = new QLineEdit();
    optionalValue->setVisible(false);
    return optionalValue;
}

void discretisationSelection::saveDiscretisations()
{
    QFileDialog dialog;
    dialog.setDefaultSuffix(".txt");
    if(!dialog.exec()) {
		return;
	}

    QString filename = dialog.selectedFiles()[0];
	if(filename.isNull()) {
		throw std::invalid_argument("No proper filename specified");
	}

    std::ofstream output;
    output.open(filename.toStdString());
    for (int i = 1; i < ui->gridLayout->rowCount(); i++){
        if (optionalValues_[i-1]->text() != ""){
            output<<i-1<<"\t"<<boxes_[i-1]->currentIndex()<<"\t"<<optionalValues_[i-1]->text().toStdString()<<std::endl;
        }
        else{
            output<<i-1<<"\t"<<boxes_[i-1]->currentIndex()<<std::endl;
        }
    }

    output.close();
}

void discretisationSelection::clicked(QAbstractButton* btn)
{
	switch(ui->dialogButtons->standardButton(btn)) {
		case QDialogButtonBox::Save:
			saveDiscretisations();
			break;
		case QDialogButtonBox::Ok:
			accept();
			break;
		case QDialogButtonBox::Cancel:
			reject();
			break;
		default:
			break;
	}
}

void discretisationSelection::box_Index_Changed(uint position, int index)
{
   if ((index == 6) || (index == 7)){
       optionalValues_[position]->setVisible(true);
       optionalValues_[position]->clear();
   }
   else{
       optionalValues_[position]->setVisible(false);
       optionalValues_[position]->clear();
   }
}
