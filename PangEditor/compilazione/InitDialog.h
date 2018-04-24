/****************************************************************************
** Form interface generated from reading ui file 'InitDialog.ui'
**
** Created: lun 21. mar 02:17:53 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.2.0   edited May 19 14:22 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef INITDIALOG_H
#define INITDIALOG_H

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
class QPushButton;

class InitDialog : public QDialog
{
    Q_OBJECT

public:
    InitDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~InitDialog();

    QGroupBox* groupBox4;
    QLabel* pixmapLabel1;
    QLabel* textLabel1;
    QLabel* pixmapLabel2;
    QLabel* textLabel2;
    QSpinBox* spinX;
    QLabel* textLabel3;
    QSpinBox* spinZ;
    QLabel* textLabel4;
    QSpinBox* spinY;
    QPushButton* pushButton;

    PangEditor* pangEditor;

public slots:
    virtual void closeUp();

protected:
    QGridLayout* InitDialogLayout;
    QVBoxLayout* layout21;
    QHBoxLayout* groupBox4Layout;
    QHBoxLayout* layout13;
    QGridLayout* layout20;
    QVBoxLayout* layout17;
    QHBoxLayout* layout14;
    QHBoxLayout* layout15;
    QHBoxLayout* layout16;

protected slots:
    virtual void languageChange();

};

#endif // INITDIALOG_H
