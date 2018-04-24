/****************************************************************************
** Form interface generated from reading ui file 'mainform.ui'
**
** Created: lun 21. mar 02:17:56 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.2.0   edited May 19 14:22 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef MAINFORM_H
#define MAINFORM_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qmainwindow.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qfile.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class PangEditor;
class ViewEditor;
class QTabWidget;
class QWidget;
class QGroupBox;
class QLabel;
class QSlider;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QFrame;
class QButtonGroup;
class QRadioButton;
class QComboBox;
class QSpinBox;
class QLCDNumber;

class Mainform : public QMainWindow
{
    Q_OBJECT

public:
    Mainform( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~Mainform();

    PangEditor* pangEditor;
    QTabWidget* tabWidget;
    QWidget* tab;
    QGroupBox* groupBox82;
    QLabel* textLabel3_3_2;
    QLabel* pixmapLabel1;
    QLabel* textLabel2;
    QLabel* textLabel3;
    QLabel* textLabel1;
    QSlider* sliR;
    QSlider* sliB;
    QSlider* sliV;
    QPushButton* addBalls;
    QPushButton* delBalls;
    QLineEdit* editTextureBall;
    QPushButton* browseTextureBall;
    QCheckBox* enableTextureBall;
    QWidget* tab_2;
    QGroupBox* groupBox83_2;
    QLabel* textLabel3_3;
    QLabel* pixmapLabel2;
    QSlider* sliB2;
    QSlider* sliV2;
    QLabel* textLabel1_2;
    QLabel* textLabel3_2;
    QLabel* textLabel2_2;
    QSlider* sliR2;
    QPushButton* addFloor;
    QPushButton* delFloor;
    QCheckBox* enableTextureFloor;
    QLineEdit* editTextureFloor;
    QPushButton* browseTextureFloor;
    QFrame* line2;
    QPushButton* addStair;
    QPushButton* delStair;
    QWidget* TabPage;
    QGroupBox* groupBox84;
    QLabel* textLabel3_3_3;
    QLabel* pixmapLabel2_2;
    QLabel* textLabel3_2_2;
    QSlider* sliB3;
    QSlider* sliR3;
    QSlider* sliV3;
    QLabel* textLabel2_2_2;
    QLabel* textLabel1_2_2;
    QPushButton* addWall;
    QPushButton* delWall;
    QCheckBox* enableTextureWall;
    QLineEdit* editTextureWall;
    QPushButton* browseTextureWall;
    QWidget* TabPage_2;
    QGroupBox* groupBox85;
    QLabel* textLabel3_3_2_2;
    QButtonGroup* buttonGroup2;
    QRadioButton* radioPlayerUno;
    QRadioButton* radioPlayerDue;
    QLabel* textLabel1_3;
    QLineEdit* editPlayerModel;
    QPushButton* browsePlayerModel;
    QLabel* textLabel1_3_2;
    QLineEdit* editPlayerTexture;
    QPushButton* browsePlayerTexture;
    QPushButton* pos;
    QComboBox* visPlayer;
    QWidget* TabPage_3;
    QGroupBox* groupBox80;
    QLabel* textLabel1_5;
    QButtonGroup* buttonGroup1;
    QRadioButton* radioView0;
    QRadioButton* radioView1;
    QRadioButton* radioView2;
    QRadioButton* radioView3;
    QRadioButton* radioView4;
    QRadioButton* radioView5;
    QLabel* textLabel2_3;
    QSpinBox* viewX;
    QSpinBox* viewY;
    QSpinBox* viewZ;
    QPushButton* viewApply;
    ViewEditor* view;
    QWidget* TabPage_4;
    QGroupBox* groupBox6;
    QLabel* textLabel3_3_2_2_2_2;
    QButtonGroup* buttonGroup3;
    QLCDNumber* LCDBonus;
    QLabel* textLabel1_9;
    QLabel* textLabel3_4;
    QLCDNumber* LCDPowerUp;
    QLabel* textLabel4;
    QLCDNumber* LCDMalus;
    QSlider* sliderPowerUp;
    QSlider* sliderBonus;
    QButtonGroup* buttonGroup5;
    QLabel* pixmapLabel2_3;
    QLabel* textLabel5;
    QComboBox* comboBoxArmi;
    QLabel* pixmapLabel3;
    QLabel* textLabel9;
    QComboBox* comboBoxFrutta;
    QLabel* pixmapLabel4;
    QLabel* textLabel10;
    QComboBox* comboBoxVite;
    QButtonGroup* buttonGroup6;
    QLabel* pixmapLabel5;
    QLabel* textLabel11;
    QComboBox* comboBoxBombe;
    QLabel* pixmapLabel6;
    QLabel* textLabel12;
    QComboBox* comboBoxTornado;
    QPushButton* pushButtonProb;
    QMenuBar *MenuBar;
    QPopupMenu *fileMenu;
    QPopupMenu *helpMenu;
    QToolBar *Toolbar;
    QAction* ActionfileNew;
    QAction* ActionfileOpen;
    QAction* ActionfileSave;
    QAction* ActionfileSaveAs;
    QAction* ActionfileExit;
    QAction* ActionhelpAbout;
    QAction* ActionhelpGuide;
    QAction* ActionfileSaveScenario;
    QAction* ActionPause;
    QAction* ActionRewind;
    QAction* ActionZoomIn;
    QAction* ActionZoomOut;
    QAction* ActionTrackball;
    QAction* ActionShadows;
    QAction* ActionCubemap;
    QAction* ActionSnapshot;

public slots:
    virtual void Zero();
    virtual void Zero2();
    virtual void fileNew();
    virtual void fileOpen();
    virtual void fileSave();
    virtual void fileSaveAs();
    virtual void fileSaveScenario();
    virtual void editBalls();
    virtual void editFloor();
    virtual void editWall();
    virtual void setBallTexture();
    virtual void setFloorTexture();
    virtual void setWallTexture();
    virtual void chooseBallTexture();
    virtual void chooseFloorTexture();
    virtual void chooseWallTexture();
    virtual void getPlayer();
    virtual void setPlayerTexture();
    virtual void setPlayerModel();
    virtual void setViews();
    virtual void snapShot();
    virtual void chooseCubemap();
    virtual void setMalus( int b );
    virtual void setProbabilita();
    virtual void fileExit();
    virtual void helpGuide();
    virtual void helpAbout();

protected:
    QGridLayout* MainformLayout;
    QGridLayout* layout49;
    QVBoxLayout* layout58;
    QVBoxLayout* tabLayout;
    QVBoxLayout* layout404;
    QVBoxLayout* groupBox82Layout;
    QHBoxLayout* layout78;
    QGridLayout* layout60;
    QHBoxLayout* layout219;
    QGridLayout* layout197;
    QHBoxLayout* layout196;
    QGridLayout* tabLayout_2;
    QVBoxLayout* layout406;
    QHBoxLayout* groupBox83_2Layout;
    QHBoxLayout* layout56;
    QGridLayout* layout200;
    QHBoxLayout* layout201;
    QVBoxLayout* layout203;
    QHBoxLayout* layout202;
    QHBoxLayout* layout201_2;
    QGridLayout* TabPageLayout;
    QVBoxLayout* layout407;
    QHBoxLayout* groupBox84Layout;
    QHBoxLayout* layout51;
    QGridLayout* layout208;
    QHBoxLayout* layout207;
    QVBoxLayout* layout206;
    QHBoxLayout* layout205;
    QGridLayout* TabPageLayout_2;
    QVBoxLayout* layout45;
    QHBoxLayout* groupBox85Layout;
    QHBoxLayout* layout49_2;
    QHBoxLayout* buttonGroup2Layout;
    QHBoxLayout* layout38;
    QVBoxLayout* layout68;
    QHBoxLayout* layout65;
    QVBoxLayout* layout67;
    QHBoxLayout* layout64;
    QHBoxLayout* layout63;
    QGridLayout* TabPageLayout_3;
    QVBoxLayout* layout58_2;
    QHBoxLayout* groupBox80Layout;
    QVBoxLayout* buttonGroup1Layout;
    QVBoxLayout* layout57;
    QVBoxLayout* layout76;
    QHBoxLayout* layout161;
    QHBoxLayout* layout169;
    QGridLayout* TabPageLayout_4;
    QVBoxLayout* layout57_2;
    QHBoxLayout* groupBox6Layout;
    QHBoxLayout* layout40_2;
    QGridLayout* buttonGroup3Layout;
    QGridLayout* layout56_2;
    QGridLayout* buttonGroup5Layout;
    QHBoxLayout* layout51_2;
    QHBoxLayout* layout52;
    QHBoxLayout* layout53;
    QGridLayout* buttonGroup6Layout;
    QHBoxLayout* layout54;
    QHBoxLayout* layout55;
    QHBoxLayout* layout60_2;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;
    QPixmap image1;

};

#endif // MAINFORM_H
