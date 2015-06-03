#ifndef DATAMATRIXMODEL_H
#define DATAMATRIXMODEL_H

#include <QtCore/QAbstractItemModel>
#include <memory>

class QObject;
class QModelIndex;

template <typename T> class Matrix;

class DataMatrixModel : public QAbstractTableModel
{
	Q_OBJECT

	public:
	using SMatrix = Matrix<std::string>;
	DataMatrixModel(const QString& sampleFile, QObject* parent);

	const std::shared_ptr<SMatrix>& getMatrix() const;
	const QString& getSampleFile() const;

	void setMatrix(const std::shared_ptr<SMatrix>& matrix);

	int rowCount(const QModelIndex& parent) const override;
	int columnCount(const QModelIndex& parent) const override;

	QVariant headerData(int section, Qt::Orientation orientation,
	                    int role = Qt::DisplayRole) const override;

	QVariant data(const QModelIndex& index,
	              int role = Qt::DisplayRole) const override;

	protected:
	QVariant displayHeaderData(int section, Qt::Orientation orientation) const;
	QVariant displayData(const QModelIndex& index) const;

	private:
	QString sampleFile_;
	std::shared_ptr<SMatrix> matrix_;
};

#endif // DATAMATRIXMODEL_H

