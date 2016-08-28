#ifndef METHODCOMBOBOX_H
#define METHODCOMBOBOX_H

#include <QtWidgets/QComboBox>

class methodComboBox : public QComboBox
{
    Q_OBJECT

public:
    /**
    * @brief methodComboBox
    * Constructor of the class methodComboBox
    * @param id Index of the methodComboBox
    */
    methodComboBox(unsigned int id);

    void setCurrentMethod(const std::string& methodName);

signals:
    /**
     * @brief newMethodSelected
     * Emits if the index of the selected item changed
     * @param index The index of the comboBox
     * @param item Name of the new item
     */
    void newMethodSelected(unsigned int index, const QString& item);

public slots:
    /**
     * @brief box_Index_Changed
     * Slot reacting on the itemIndexChangedEvent
     * @param index New item index
     */
    void box_Index_Changed(int index);

private:
    /**
     *Index of the ComboBox
     */
    unsigned int id_;
};

#endif // METHODCOMBOBOX_H
