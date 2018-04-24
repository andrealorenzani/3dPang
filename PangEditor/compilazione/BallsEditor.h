/****************************************************************************
** Form interface generated from reading ui file 'BallsEditor.ui'
**
** Created: lun 21. mar 02:17:50 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.2.0   edited May 19 14:22 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef BALLSEDITOR_H
#define BALLSEDITOR_H

#include <qvariant.h>
#include <qdialog.h>
#include <qfiledialog.h>
#include "PangEditor.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QGroupBox;
class QLabel;
class QSpinBox;
class QComboBox;
class QSlider;
class QCheckBox;
class QLineEdit;
class QPushButton;
class QLCDNumber;

class ballsEditor : public QDialog
{
    Q_OBJECT

public:
    ballsEditor( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ballsEditor();

    QGroupBox* groupBox9;
    QLabel* pixmapLabel1;
    QLabel* textLabel1_3_2;
    QLabel* pixmapLabel5;
    QGroupBox* groupBox15;
    QLabel* textLabel1;
    QSpinBox* spinBox1;
    QLabel* textLabel6_2;
    QSpinBox* spinBox5;
    QLabel* textLabel1_3;
    QSpinBox* spinBox4;
    QLabel* textLabel3;
    QComboBox* comboBox1;
    QSpinBox* spinBox3;
    QLabel* textLabel2;
    QSpinBox* spinBox2;
    QGroupBox* colPalle;
    QSlider* sliV;
    QLabel* textLabel2_2;
    QSlider* sliR;
    QLabel* textLabel1_2;
    QLabel* textLabel3_2;
    QSlider* sliB;
    QGroupBox* groupBox8;
    QCheckBox* enableTexture;
    QLineEdit* editTexture;
    QPushButton* browseTexture;
    QGroupBox* groupBox5;
    QLabel* textLabel1_4;
    QLabel* textLabel6;
    QSlider* sliX;
    QLCDNumber* lCDNumber1;
    QLabel* textLabel7;
    QSlider* sliZ;
    QLCDNumber* lCDNumber1_2;
    QPushButton* pushButton5;

    PangEditor* pangEditor;

public slots:
    virtual void setGrav( int g );
    virtual void init( PangEditor * pang );
    virtual void closeUp();
    virtual void chooseTexture();

protected:
    QGridLayout* ballsEditorLayout;
    QVBoxLayout* layout82;
    QHBoxLayout* groupBox9Layout;
    QHBoxLayout* layout74;
    QVBoxLayout* groupBox15Layout;
    QVBoxLayout* layout80;
    QHBoxLayout* layout75;
    QHBoxLayout* layout76;
    QHBoxLayout* layout77;
    QHBoxLayout* layout78;
    QHBoxLayout* layout79;
    QHBoxLayout* layout70;
    QVBoxLayout* colPalleLayout;
    QGridLayout* layout17;
    QVBoxLayout* groupBox8Layout;
    QVBoxLayout* layout19;
    QHBoxLayout* layout18;
    QGridLayout* layout81;
    QGridLayout* groupBox5Layout;
    QVBoxLayout* layout22;
    QHBoxLayout* layout20;
    QHBoxLayout* layout21;

protected slots:
    virtual void languageChange();

};

#endif // BALLSEDITOR_H
