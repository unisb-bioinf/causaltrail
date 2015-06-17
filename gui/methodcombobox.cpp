#include "methodcombobox.h"

methodComboBox::methodComboBox(unsigned int id):
    id_(id)
{
    addItem("Ceil");
    addItem("Floor");
    addItem("Round to nearest");
    addItem("Arithmetic Mean");
    addItem("Harmonic Mean");
    addItem("Median");
    addItem("Manual Threshold");
    addItem("BracketMedians");
    addItem("PearsonTukey");
    addItem("Is Discrete");
    addItem("Z-Score");
    setCurrentIndex(9);
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(box_Index_Changed(int)));
}

void methodComboBox::box_Index_Changed(int index)
{
    emit(newMethodSelected(id_,index));
}
