#include "listwidgetmulticopy.h"
#include "QApplication"
#include "QClipboard"
#include "QKeyEvent"

ListWidgetMultiCopy::ListWidgetMultiCopy(QWidget *parent)
{

}

void ListWidgetMultiCopy::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Copy) {
        QString text;
        for (auto it : this->selectedItems()){
            text+=it->text()+"\n";
        }
        QApplication::clipboard()->setText(text);
    }
}
