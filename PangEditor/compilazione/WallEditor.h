/****************************************************************************
** Form interface generated from reading ui file 'WallEditor.ui'
**
** Created: lun 21. mar 02:17:54 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.2.0   edited May 19 14:22 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef WALLEDITOR_H
#define WALLEDITOR_H

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
class QLineEdit;
class QCheckBox;
class QPushButton;
class QSpinBox;
class QButtonGroup;
class QRadioButton;

class wallEditor : public QDialog
{
    Q_OBJECT

public:
    wallEditor( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~wallEditor();

    QGroupBox* colPiani;
    QLabel* textLabel3_2;
    QSlider* sliB;
    QSlider* sliR;
    QLabel* textLabel1_2;
    QLabel* textLabel2_2;
    QSlider* sliV;
    QGroupBox* groupBox8;
    QLineEdit* editTexture;
    QCheckBox* enableTexture;
    QPushButton* browseTexture;
    QGroupBox* groupBox6;
    QLabel* textLabel3;
    QSpinBox* spinLUN;
    QLabel* textLabel2;
    QSpinBox* spinLAR;
    QLabel* textLabel1;
    QSpinBox* spinHEIGHT;
    QLabel* textLabel5;
    QSpinBox* spinHIGH;
    QButtonGroup* buttonGroup1;
    QRadioButton* radioFisso;
    QRadioButton* radioEliminabile;
    QPushButton* pushButton16;
    QGroupBox* groupBox25;
    QLabel* pixmapLabel4;
    QLabel* textLabel1_3;
    QLabel* pixmapLabel2;

public slots:
    virtual void init( PangEditor * pang );
    virtual void closeUp();
    virtual void chooseTexture();

protected:
    PangEditor* pangEditor;

    QGridLayout* wallEditorLayout;
    QGridLayout* layout74;
    QHBoxLayout* layout339;
    QVBoxLayout* colPianiLayout;
    QGridLayout* layout65;
    QVBoxLayout* groupBox8Layout;
    QGridLayout* layout338;
    QGridLayout* groupBox6Layout;
    QVBoxLayout* layout73;
    QVBoxLayout* layout70;
    QHBoxLayout* layout68;
    QHBoxLayout* layout69;
    QVBoxLayout* buttonGroup1Layout;
    QHBoxLayout* layout46;
    QHBoxLayout* groupBox25Layout;
    QHBoxLayout* layout336;

protected slots:
    virtual void languageChange();

};

#endif // WALLEDITOR_H
