#ifndef DATAVIEW_H
#define DATAVIEW_H

#include "../core/Matrix.h"
#include <QDialog>

namespace Ui {
	class DataView;
}

class DataMatrixModel;
class QPushButton;

class dataview : public QDialog
{
    Q_OBJECT

public:
	/**
	 * @brief dataview
	 * Constructs a dataview object
	 * @param parent, the parent widget
	 * @param samples, name of the file containing samples
	 * @return a dataview object
	 */
    explicit dataview(QWidget *parent = 0, const QString& samples = "");

	/**
	 * @brief ~dataview
	 * Destructor of the dataview object
	 */
    ~dataview();

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
};

#endif // DATAVIEW_H

