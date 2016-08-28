#ifndef DATAVIEW_H
#define DATAVIEW_H

#include "../core/Matrix.h"
#include <QtWidgets/QDialog>

namespace Ui {
	class DataView;
}

class DataMatrixModel;
class QPushButton;

class DataView : public QDialog
{
    Q_OBJECT

public:
	/**
	 * Constructs a DataView object
	 *
	 * @param parent the parent widget
	 * @param samples name of the file containing samples
	 */
    explicit DataView(QWidget *parent = 0, const QString& samples = "");

	/**
	 * Destructor of the DataView object
	 */
    ~DataView();

public slots:
	void accept() override;

protected slots:
	void selectAllColumns_();
	void deselectAllColumns_();

signals:
	void dataAccepted(const QString& sampleFile, std::vector<uint> toDelete);

private:
	//UserInterface
	Ui::DataView *ui;
	QPushButton* selectAll_;
	QPushButton* deselectAll_;

	// Underlying model for the Matrix
	DataMatrixModel* model_;

	const std::vector<unsigned int> getDeselectedSamples() const;

	QSize sizeHint() const override;
};

#endif // DATAVIEW_H

