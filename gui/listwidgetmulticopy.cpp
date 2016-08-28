#include "listwidgetmulticopy.h"

#include <QtWidgets/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QKeyEvent>
#include <QtGui/QKeySequence>

ListWidgetMultiCopy::ListWidgetMultiCopy(QWidget *parent)
{

}

void ListWidgetMultiCopy::keyPressEvent(QKeyEvent *event)
{
    if(event->matches(QKeySequence::Copy)){
        QString text;
        for (auto it : this->selectedItems()){
            text+=it->text()+"\n";
        }
        QApplication::clipboard()->setText(text);
   }
}
