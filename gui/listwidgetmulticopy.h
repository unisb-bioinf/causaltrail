#ifndef LISTWIDGETMULTICOPY_H
#define LISTWIDGETMULTICOPY_H

#include <QListWidget>

namespace Ui {
class  ListWidgetMultiCopy;
}

class ListWidgetMultiCopy : public QListWidget
{
    Q_OBJECT
public:
	/**
	 * @brief ListWidgetMultiCopy Constructor
	 * @param ParentWidget
	 */
    explicit ListWidgetMultiCopy(QWidget *parent = 0);

private slots:
	/**
	 * @brief keyPressEvent
	 * Reimplementation of the keyPressEvent. Reimplements the
	 * handling of Strg+C to copy all selected lines
	 */
    virtual void keyPressEvent(QKeyEvent *event);

};

#endif // LISTWIDGETMULTICOPY_H
