/****************************************************************************
** Form interface generated from reading ui file 'EnvEditor.ui'
**
** Created: lun 21. mar 02:17:52 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.2.0   edited May 19 14:22 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ENVEDITOR_H
#define ENVEDITOR_H

#include <qvariant.h>
#include <qdialog.h>
#include <qfiledialog.h>
#include "PangEditor.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QGroupBox;
class QLabel;
class QSlider;
class QCheckBox;
class QPushButton;
class QLineEdit;
class QSpinBox;
class QButtonGroup;
class QRadioButton;

class envEditor : public QDialog
{
    Q_OBJECT

public:
    envEditor( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~envEditor();

    QGroupBox* groupBox15;
    QLabel* pixmapLabel4;
    QLabel* textLabel1_3;
    QLabel* pixmapLabel1;
    QGroupBox* colPiani;
    QLabel* textLabel3_2;
    QSlider* sliV;
    QSlider* sliB;
    QLabel* textLabel2_2;
    QLabel* textLabel1_2;
    QSlider* sliR;
    QGroupBox* groupBox8;
    QCheckBox* enableTexture;
    QPushButton* browseTexture;
    QLineEdit* editTexture;
    QGroupBox* groupBox6;
    QLabel* textLabel3;
    QSpinBox* spinX;
    QLabel* textLabel2;
    QSpinBox* spinZ;
    QLabel* textLabel1;
    QSpinBox* spinSP;
    QLabel* textLabel5;
    QSpinBox* spinH;
    QButtonGroup* buttonGroup1;
    QRadioButton* radioFisso;
    QRadioButton* radioEliminabile;
    QPushButton* pushButton16;

    PangEditor* pangEditor;

public slots:
    virtual void init( PangEditor * pang );
    virtual void closeUp();
    virtual void chooseTexture();

protected:
    int posAmb;

    QVBoxLayout* envEditorLayout;
    QVBoxLayout* layout69;
    QHBoxLayout* groupBox15Layout;
    QHBoxLayout* layout127;
    QHBoxLayout* layout126;
    QVBoxLayout* colPianiLayout;
    QGridLayout* layout41;
    QHBoxLayout* layout36;
    QVBoxLayout* groupBox8Layout;
    QGridLayout* layout124;
    QHBoxLayout* layout68;
    QHBoxLayout* groupBox6Layout;
    QVBoxLayout* layout47;
    QHBoxLayout* layout34;
    QHBoxLayout* layout33;
    QHBoxLayout* buttonGroup1Layout;
    QHBoxLayout* layout46;

protected slots:
    virtual void languageChange();

};

#endif // ENVEDITOR_H
