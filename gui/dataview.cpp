#include "dataview.h"
#include "ui_DataView.h"

#include "DataMatrixModel.h"
#include <qpushbutton.h>

dataview::dataview(QWidget *parent, const QString& samples)
	: QDialog(parent),
      ui(new Ui::DataView)
{
    ui->setupUi(this);
    ui->matrixView->showGrid();

	model_ = new DataMatrixModel(samples, this);
	ui->matrixView->setModel(model_);

	selectAllColumns_();

	selectAll_ = new QPushButton(this);
	selectAll_->setIcon(QIcon::fromTheme("edit-select-all"));
	selectAll_->setText(tr("Select all"));
	connect(selectAll_, SIGNAL(clicked()), this, SLOT(selectAllColumns_()));
	ui->buttons->addButton(selectAll_, QDialogButtonBox::ActionRole);

	deselectAll_ = new QPushButton(this);
	deselectAll_->setIcon(QIcon::fromTheme("edit-delete"));
	deselectAll_->setText(tr("Deselect all"));
	connect(deselectAll_, SIGNAL(clicked()), this, SLOT(deselectAllColumns_()));
	ui->buttons->addButton(deselectAll_, QDialogButtonBox::ActionRole);
}

void dataview::selectAllColumns_()
{
	const int columnCount = model_->columnCount(QModelIndex());
	QItemSelection selection(model_->index(0,0), model_->index(0, columnCount - 1));
	ui->matrixView->selectionModel()->select(selection, QItemSelectionModel::Select | QItemSelectionModel::Columns);
}

void dataview::deselectAllColumns_()
{
	const int columnCount = model_->columnCount(QModelIndex());
	QItemSelection selection(model_->index(0,0), model_->index(0, columnCount - 1));
	ui->matrixView->selectionModel()->select(selection, QItemSelectionModel::Deselect | QItemSelectionModel::Columns);
}

dataview::~dataview()
{
	delete ui;
}

void dataview::accept()
{
	emit dataAccepted(model_->getSampleFile(), getDeselectedSamples());
	QDialog::accept();
}

const std::vector<unsigned int> dataview::getDeselectedSamples() const {
	std::vector<unsigned int> deselected;

	const int columnCount = model_->columnCount(QModelIndex());

	for(int i = 0; i < columnCount; ++i) {
		if(!ui->matrixView->selectionModel()->isColumnSelected(i, QModelIndex())) {
			deselected.push_back(i);
		}
	}

	return deselected;
}

