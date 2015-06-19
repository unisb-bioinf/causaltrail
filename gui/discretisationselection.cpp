#include "discretisationselection.h"
#include "ui_discretisationselection.h"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

discretisationSelection::discretisationSelection(QWidget* parent)
    : QDialog(parent), index_(-1), ui(new Ui::DiscretisationSelection)
{
	ui->setupUi(this);

	intValidator_ = new QIntValidator(this);
	doubleValidator_ = new QDoubleValidator(this);

	QPushButton* button =
	    ui->dialogButtons->addButton(tr("Load"), QDialogButtonBox::ActionRole);

	connect(button, &QPushButton::clicked, this, &discretisationSelection::loadControlFile);
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

std::string
discretisationSelection::getParameterName_(const std::string& method) const
{
	if(method == "threshold") {
		return "threshold";
	} else if(method == "bracketMedians") {
		return "buckets";
	}

	return "";
}

void discretisationSelection::generatePropertyTree() 
{
	propertyTree_ = DiscretisationSettings();
	for(int i = 1; i < ui->gridLayout->rowCount(); i++) {
		auto featureName = featureNames_[i - 1]->text().toStdString();
		auto method =
		    qvariant_cast<QString>(boxes_[i - 1]->currentData()).toStdString();

		std::string parameterName = getParameterName_(method);

		if(parameterName != "") {
			const std::string parameter = optionalValues_[i-1]->text().toStdString();
			propertyTree_.addToTree(featureName, method, parameterName, parameter);
		} else {
			propertyTree_.addToTree(featureName, method);
		}
	}
}

DiscretisationSettings& discretisationSelection::getPropertyTree(){
	return propertyTree_;
}

int discretisationSelection::index() const { return index_; }

const std::string discretisationSelection::getControlFileName() const { return controlFileName_.toStdString(); }

void discretisationSelection::adaptGUIToData() {
	featureNames_.clear();
	optionalValues_.clear();

	for(QObject* o : ui->gridLayout->children()) {
		o->deleteLater();
	}

	ui->gridLayout->addWidget(new QLabel(tr("Variable name")), 0, 0, 0);
	ui->gridLayout->addWidget(new QLabel(tr("Discretisation method")), 0, 1, 0);
	ui->gridLayout->addWidget(new QLabel(tr("Threshold/Number of bins")), 0, 2, 0);

    methodComboBox* methodSelection;
    QLineEdit* optionalValue;
    Matrix<std::string> originalData(samples_.toStdString(),false,true);
    for (unsigned int i = 1; i < originalData.getRowCount()+1; i++){
        QLabel* featureName = new QLabel(QString::fromStdString(originalData.getRowNames()[i-1]));
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
}

discretisationSelection::~discretisationSelection()
{
    delete ui;
}

methodComboBox* discretisationSelection::newMethodSelection(unsigned int i)
{
    methodComboBox* methodSelection = new methodComboBox(i);
    connect(methodSelection, &methodComboBox::newMethodSelected, this,
            &discretisationSelection::box_Index_Changed);
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

void discretisationSelection::loadControlFile() {
	QString fileName = QFileDialog::getOpenFileName(
	    this, "Select a discretisation control file", "", "*.json");

	if(fileName.isNull()) {
		return;
	}

	propertyTree_ = DiscretisationSettings(fileName.toStdString());

	for(size_t i = 0; i < featureNames_.size(); ++i) {
		std::string nodeName = featureNames_[i]->text().toStdString();
		if(propertyTree_.containsNode(nodeName)) {
			boxes_[i]->setCurrentMethod(propertyTree_.getMethod(nodeName));
		}
	}
}

void discretisationSelection::clicked(QAbstractButton* btn)
{
	switch(ui->dialogButtons->standardButton(btn)) {
		case QDialogButtonBox::Save:
			saveDiscretisations();
			break;
		case QDialogButtonBox::Ok:
			generatePropertyTree();
			accept();
			break;
		case QDialogButtonBox::Cancel:
			reject();
			break;
		default:
			break;
	}
}

void discretisationSelection::box_Index_Changed(uint position, const QString& method)
{
	QLineEdit* box = optionalValues_[position];

	if(method == "threshold") {
		box->setValidator(doubleValidator_);
		box->setText("0.0");
		box->show();
	} else if(method == "bracketMedians") {
		box->setValidator(intValidator_);
		box->setText("0");
		box->show();
	} else {
		box->hide();
	}
}
