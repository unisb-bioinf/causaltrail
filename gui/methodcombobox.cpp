#include "methodcombobox.h"

methodComboBox::methodComboBox(unsigned int id) : id_(id)
{
    addItem("Ceil", "ceil");
    addItem("Floor", "floor");
    addItem("Round to nearest", "round");
    addItem("Arithmetic Mean", "arithmeticMean");
    addItem("Harmonic Mean", "harmonicMean");
    addItem("Median", "median");
    addItem("Manual Threshold", "threshold");
    addItem("BracketMedians", "bracketMedians");
    addItem("PearsonTukey", "pearsonTukey");
    addItem("Is Discrete", "none");
    addItem("Z-Score", "zScore");
    setCurrentIndex(9);
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(box_Index_Changed(int)));
}

void methodComboBox::setCurrentMethod(const std::string& methodName) {
	int index = findData(QString::fromStdString(methodName), Qt::UserRole,
	                     Qt::MatchFixedString);

	if(index != -1) {
		setCurrentIndex(index);
	}
}

void methodComboBox::box_Index_Changed(int index)
{
    emit(newMethodSelected(id_, qvariant_cast<QString>(currentData())));
}
