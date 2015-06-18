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
	controlFileName_="";

	adaptGUIToData();
	QDialog::show();
}

const QString& discretisationSelection::samples() const { return samples_; }

const std::string discretisationSelection::getParameterNames(unsigned int index){
	switch(index){
		case 6 : {
			return "threshold";
		}
		case 7: {
			return "buckets";
		}
	}
	throw std::invalid_argument("Index unknown");
}

void discretisationSelection::generatePropertyTree() 
{
	propertyTree_ = DiscretisationSettings ();
	for (int i = 1; i < ui->gridLayout->rowCount(); i++){
		const std::string featureName = featureNames_[i-1]->text().toStdString();	
		unsigned int methodIndex = boxes_[i-1]->currentIndex();	
		const std::string method = methodNames_[methodIndex];
		if ((methodIndex == 6) or (methodIndex == 7)){
			const std::string parameter = optionalValues_[i-1]->text().toStdString();
			propertyTree_.addToTree(featureName,method,getParameterNames(methodIndex),parameter);
		}
		else {
			propertyTree_.addToTree(featureName,method);
		}
	}
}

DiscretisationSettings& discretisationSelection::getPropertyTree(){
	return propertyTree_;
}

int discretisationSelection::index() const { return index_; }

const std::string discretisationSelection::getControlFileName() const { return controlFileName_.toStdString(); }

void discretisationSelection::adaptGUIToData(){
    QLineEdit* featureName;
    methodComboBox* methodSelection;
    QLineEdit* optionalValue;
    Matrix<std::string> originalData(samples_.toStdString(),false,true);
    for (unsigned int i = 1; i < originalData.getRowCount()+1; i++){
        featureName = newFeatureName(QString::fromStdString(originalData.getRowNames()[i-1]));
	featureNames_.push_back(featureName);
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
    dialog.setDefaultSuffix(".json");
    if(!dialog.exec()) {
		return;
	}

    QString fileName = dialog.selectedFiles()[0];
	if(fileName.isNull()) {
		throw std::invalid_argument("No proper filename specified");
	}	
    controlFileName_=fileName;
    generatePropertyTree();
    propertyTree_.exportToFile(controlFileName_.toStdString());
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
