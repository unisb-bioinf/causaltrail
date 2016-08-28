#ifndef LISTWIDGETMULTICOPY_H
#define LISTWIDGETMULTICOPY_H

#include <QtWidgets/QListWidget>

namespace Ui {
class  ListWidgetMultiCopy;
}

class ListWidgetMultiCopy : public QListWidget
{
    Q_OBJECT
public:
	/**
	 * Constructor
	 *
	 * @param parent Pointer to parent widget.
	 */
    ListWidgetMultiCopy(QWidget* parent = 0);

private slots:
	/**
	 * Reimplementation of the keyPressEvent. Reimplements the
	 * handling of Strg+C to copy all selected lines
	 */
    virtual void keyPressEvent(QKeyEvent *event);

};

#endif // LISTWIDGETMULTICOPY_H
