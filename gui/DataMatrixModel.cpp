#include "DataMatrixModel.h"

#include "../core/Matrix.h"

DataMatrixModel::DataMatrixModel(const QString& sampleFile, QObject* parent)
    : QAbstractTableModel(parent), sampleFile_(sampleFile)
{
	matrix_ = std::make_shared<SMatrix>(sampleFile_.toStdString(), false, true);
}

int DataMatrixModel::rowCount(const QModelIndex&) const
{
	return matrix_->getRowCount();
}

int DataMatrixModel::columnCount(const QModelIndex&) const
{
	return matrix_->getColCount();
}

void DataMatrixModel::setMatrix(const std::shared_ptr<SMatrix>& matrix)
{
	matrix_ = matrix;
}

const QString& DataMatrixModel::getSampleFile() const { return sampleFile_; }

const std::shared_ptr<Matrix<std::string>>& DataMatrixModel::getMatrix() const
{
	return matrix_;
}

QVariant DataMatrixModel::data(const QModelIndex& index, int role) const
{
	switch(role) {
		case Qt::DisplayRole:
			return displayData(index);
	}

	return QVariant();
}

QVariant DataMatrixModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
	switch(role) {
		case Qt::DisplayRole:
			return displayHeaderData(section, orientation);
	}

	return QVariant();
}

QVariant DataMatrixModel::displayHeaderData(int section,
                                            Qt::Orientation orientation) const
{
	if(orientation == Qt::Horizontal) {
		if(matrix_->getColNames().empty()) {
			return QVariant(section + 1);
		} else {
			return QString::fromStdString(matrix_->getColNames()[section]);
		}
	} else {
		return QString::fromStdString(matrix_->getRowNames()[section]);
	}
}

QVariant DataMatrixModel::displayData(const QModelIndex& index) const
{
	return QString::fromStdString(
	    matrix_->getData(index.column(), index.row()));
}

