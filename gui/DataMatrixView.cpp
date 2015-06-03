#include "DataMatrixView.h"

DataMatrixView::DataMatrixView(QWidget* parent) : QTableView(parent)
{
	setSelectionMode(MultiSelection);
	setSelectionBehavior(SelectColumns);
}

