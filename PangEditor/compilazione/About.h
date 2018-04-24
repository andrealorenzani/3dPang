/****************************************************************************
** Form interface generated from reading ui file 'About.ui'
**
** Created: lun 21. mar 02:17:48 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.2.0   edited May 19 14:22 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ABOUT_H
#define ABOUT_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QPushButton;

class about : public QDialog
{
    Q_OBJECT

public:
    about( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~about();

    QLabel* textLabel2;
    QLabel* textLabel2_2;
    QLabel* textLabel3;
    QPushButton* OK;
    QLabel* pixmapLabel1;

protected:

protected slots:
    virtual void languageChange();

};

#endif // ABOUT_H
