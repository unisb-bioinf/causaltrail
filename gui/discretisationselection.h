#ifndef DISCRETISATIONSELECTION_H
#define DISCRETISATIONSELECTION_H

#include <QWidget>
#include "../core/Matrix.h"
#include "QLineEdit"
#include "methodcombobox.h"

namespace Ui {
class discretisationSelection;
}

class discretisationSelection : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief discretisationSelection
     * Constructor of class discretisationSelection
     * @param parent The parent Widget
     * @param path The standard path to save files in CausalTrail
     * @param samples The filename of the samples file
     * @param index The index of the current Session
     */
    discretisationSelection(QWidget *parent, QString path, QString samples, int index);

    /**
     * @brief adaptGUIToData
     * Adapts the GUI to fit to the samples contained in the sample file provided in the constructor
     */
    void adaptGUIToData();

    /**
     * @brief ~discretisationSelection()
     * Destructor of this class
     */
    ~discretisationSelection();

signals:
    /**
     * @brief fileSaved
     * Emits if a control file is saved
     */
    void fileSaved(QString,QString,int);

private slots:
    /**
     * @brief on_saveDiscretisation_clicked
     * Saves the selected discretisation methods in a user specified file
     */
    void on_saveDiscretisation_clicked();

    /**
     * @brief box_Index_Changed
     * @param position Index of the ComboBox that was changed
     * @param index New itemIndex of the ComboBox
     */
    void box_Index_Changed(uint position, int index);

private:
    /**
     * @brief path_
     * Standard file save path of causalTrail
     */
    QString path_;

    /**
     * @brief samples_
     * Name of the file containing samples_
     */
    QString samples_;

    /**
     * @brief index_
     * Index of the corresponding NetworkInstance
     */
    int index_;

    /**
     * @brief ui
     * Graphical User Interface
     */
    Ui::discretisationSelection *ui;

    /**
     * @brief newFeatureName
     * Creates a new QLineEdit object representing the feature with the given name
     * @param name Name of the Feature
     * @return Pointer to a QLineEdit Object
     */
    QLineEdit* newFeatureName(QString name);

    /**
     * @brief newMethodSelection
     * Creates a new MethodComboBox object representing all available discretisation methods
     * @param i Index of the methodComboBox
     * @return Pointer to a MethodComboBox Object
     */
    methodComboBox* newMethodSelection(unsigned int i);

    /**
     * @brief newOptionalValue
     * Creates a new QLineEdit object. It enables the user to enter optional values for the discretisation
     * @return Pointer to a QLineEdit Object
     */
    QLineEdit* newOptionalValue();

    /**
     * @brief boxes_
     * vector holding pointers to all MethodComboBox Objects
     */
    std::vector<methodComboBox*> boxes_;

    /**
     * @brief optionalValues_
     * vector holding pointers to all QLineEdit Objects for the optional values
     */
    std::vector<QLineEdit*> optionalValues_;


};

#endif // DISCRETISATIONSELECTION_H
